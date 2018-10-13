/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>

#include "ipc/messagecodec.h"
#include "serial/portendpoint.h"
#include "session/session.h"

PortEndpoint::PortEndpoint(QObject* parent)
    : QObject(parent)
    , process_(nullptr)
    , server_(nullptr)
    , socket_(nullptr)
{
    // currently nothing
}

void PortEndpoint::onProcessFinished(int retval)
{
    qDebug() << "[slot] onProcessFinished" << retval;

    disconnectEndpoint();
    emit disconnected(retval);
}

void PortEndpoint::onProcessStarted()
{
    qDebug() << "[slot] onProcessStarted";
}

void PortEndpoint::onProcessError(QProcess::ProcessError error)
{
    qDebug() << "[slot] onProcessError:" << error;
}

void PortEndpoint::onSocketConnection()
{
    qDebug() << "[slot] onSocketConnection";

    socket_ = server_->nextPendingConnection();

    if (socket_)
    {
        connect(socket_, &QLocalSocket::disconnected, socket_, &PortEndpoint::deleteLater);
        connect(socket_, &QLocalSocket::readyRead, this, &PortEndpoint::onSocketData);
        connect(socket_, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error), this, &PortEndpoint::onSocketError);

        emit connected();
    }
}

void PortEndpoint::onSocketData()
{
    QByteArray array = socket_->readAll();
    while (array.size() > 0)
    {
        emit readyRead(MessageCodec::decode(array));
    }
}

void PortEndpoint::onSocketError(QLocalSocket::LocalSocketError error)
{
    qDebug() << "[slot] onSocketError:" << error;
}

quint64 PortEndpoint::writeData(const QByteArray& byteArray)
{
    return write(MessageCodec::encodeData(byteArray));
}

quint64 PortEndpoint::writeSignal(const Message::Signal& signal)
{
    return write(MessageCodec::encodeSignal(signal));
}

quint64 PortEndpoint::write(const QByteArray& byteArray)
{
    quint64 u64ret = 0;

    if (socket_)
    {
        u64ret = socket_->write(byteArray);
    }

    return u64ret;
}

void PortEndpoint::disconnectEndpoint()
{
    if (socket_)
    {
        delete socket_;
        socket_ = nullptr;
    }

    if (server_)
    {
        delete server_;
        server_ = nullptr;
    }

    if (process_)
    {
        process_->deleteLater();
    }
}

void PortEndpoint::connectEndpoint(Session* session)
{
    process_ = new QProcess(this);
    server_  = new QLocalServer(this);

    connect(process_, &QProcess::started, this, &PortEndpoint::onProcessStarted);
    connect(process_, &QProcess::errorOccurred, this, &PortEndpoint::onProcessError);
    connect(process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &PortEndpoint::onProcessFinished);
    connect(server_, &QLocalServer::newConnection, this, &PortEndpoint::onSocketConnection);

    qDebug() << "LISTEN" << "serial:" + session->getDeviceName();

    QString socketName = session->getDeviceName();
    socketName = socketName.replace("/", "_");

    if (!server_->listen("serial:" + socketName))
    {
        qDebug() << "ERROR LISTEN" << server_->errorString();
    }

    QStringList args;

    args << session->getDeviceName()
         << QString::number(session->getBaudRate())
         << QString::number(session->getDataBits())
         << QString::number(session->getParity())
         << QString::number(session->getStopBits())
         << QString::number(session->getFlowControl());

    process_->setProcessChannelMode(QProcess::ForwardedChannels);
    process_->start(QCoreApplication::applicationFilePath(), args);
}

bool PortEndpoint::isConnected()
{
    return socket_ != nullptr;
}

// EOF <stefan@scheler.com>
