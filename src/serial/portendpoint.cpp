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
    , m_process(nullptr)
    , m_server(nullptr)
    , m_socket(nullptr)
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

    m_socket = m_server->nextPendingConnection();

    if (m_socket)
    {
        connect(m_socket, &QLocalSocket::disconnected, m_socket, &PortEndpoint::deleteLater);
        connect(m_socket, &QLocalSocket::readyRead, this, &PortEndpoint::onSocketData);
        connect(m_socket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error), this, &PortEndpoint::onSocketError);

        emit connected();
    }
}

void PortEndpoint::onSocketData()
{
    QByteArray array = m_socket->readAll();
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

    if (m_socket)
    {
        u64ret = m_socket->write(byteArray);
    }

    return u64ret;
}

void PortEndpoint::disconnectEndpoint()
{
    if (m_socket)
    {
        delete m_socket;
        m_socket = nullptr;
    }

    if (m_server)
    {
        delete m_server;
        m_server = nullptr;
    }

    if (m_process)
    {
        m_process->deleteLater();
    }
}

void PortEndpoint::connectEndpoint(Session* session)
{
    m_process = new QProcess(this);
    m_server  = new QLocalServer(this);

    connect(m_process, &QProcess::started, this, &PortEndpoint::onProcessStarted);
    connect(m_process, &QProcess::errorOccurred, this, &PortEndpoint::onProcessError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &PortEndpoint::onProcessFinished);
    connect(m_server, &QLocalServer::newConnection, this, &PortEndpoint::onSocketConnection);

    qDebug() << "LISTEN" << "serial:" + session->getDeviceName();

    QString socketName = session->getDeviceName();
    socketName = socketName.replace("/", "_");

    if (!m_server->listen("serial:" + socketName))
    {
        qDebug() << "ERROR LISTEN" << m_server->errorString();
    }

    QStringList args;

    args << session->getDeviceName()
         << QString::number(session->getBaudRate())
         << QString::number(session->getDataBits())
         << QString::number(session->getParity())
         << QString::number(session->getStopBits())
         << QString::number(session->getFlowControl());

    m_process->setProcessChannelMode(QProcess::ForwardedChannels);
    m_process->start(QCoreApplication::applicationFilePath(), args);
}

bool PortEndpoint::isConnected()
{
    return m_socket != nullptr;
}

// EOF <stefan@scheler.com>
