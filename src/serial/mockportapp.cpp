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

#include <QCoreApplication>
#include <QLocalSocket>
#include <QTimer>
#include <QRandomGenerator>

#include "ipc/message.h"
#include "ipc/messagecodec.h"
#include "serial/mockportapp.h"

MockPortApp::MockPortApp(int& argc, char** argv)
    : QCoreApplication(argc, argv)
    , socket_(new QLocalSocket(this))
{
    // currently empty
}

int MockPortApp::exec()
{
    QString portName = arguments().at(1);
    //quint32 baudRate = arguments().at(2).toUInt();

    connect(socket_, &QLocalSocket::connected, this, &MockPortApp::onSocketConnected);
    connect(socket_, &QLocalSocket::disconnected, this, &MockPortApp::onSocketDisconnected);
    connect(socket_, &QLocalSocket::readyRead, this, &MockPortApp::onSocketData);
    connect(socket_, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::errorOccurred), this, &MockPortApp::onSocketError);

    socket_->abort();
    socket_->connectToServer("serial:" + portName.replace("/", "_"));

    QTextStream(stdout) << "started:" << portName << Qt::endl;

    return QCoreApplication::exec();
}

void MockPortApp::onSocketData()
{
    QByteArray array = socket_->readAll();

    while (array.size() > 0)
    {
        const Message& message = MessageCodec::decode(array);

        if (message.isCmd(Message::DataCmd))
        {
            processData(message.getPayload());
        }
        else if (message.isCmd(Message::SigCmd))
        {
            if (message.getSignal() == Message::CancelConSig)
            {
                //m_observer->setActive(false);
            }
        }
    }
}

void MockPortApp::onSocketDisconnected()
{
    QTextStream(stdout) << "disconnected" << Qt::endl;
    QCoreApplication::quit();
}

void MockPortApp::onSocketConnected()
{
    QTextStream(stdout) << "connected" << Qt::endl;

    sendDummyData();
}

void MockPortApp::onSocketError(QLocalSocket::LocalSocketError error)
{
    QTextStream(stdout) << error << Qt::endl;
    QCoreApplication::quit();
}

void MockPortApp::processData(const QByteArray& data)
{
    socket_->write(MessageCodec::encodeData(data));
}

void MockPortApp::sendDummyData()
{
    socket_->write(MessageCodec::encodeData("[data] 012345678901234567890123456789012345678901234567890123456789\r\n"));

    QTimer::singleShot(QRandomGenerator::global()->bounded(0, 500), this, &MockPortApp::sendDummyData);
}

// EOF <stefan@scheler.com>
