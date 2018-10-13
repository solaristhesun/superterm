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

#ifndef CPORTENDPOINT_H
#define CPORTENDPOINT_H

#include <QLocalSocket>
#include <QObject>
#include <QProcess>

#include "ipc/message.h"

class QLocalServer;
class QLocalSocket;
class Session;

class PortEndpoint : public QObject
{
    Q_OBJECT
public:
    explicit PortEndpoint(QObject* parent = nullptr);

    void    connectEndpoint(Session* session);
    void    disconnectEndpoint();
    bool    isConnected();
    quint64 writeData(const QByteArray& byteArray);
    quint64 writeSignal(const Message::Signal& signal);
    quint64 write(const QByteArray& byteArray);

signals:
    void readyRead(const Message& message);
    void disconnected(int);
    void connected();
    void showStatusBar();
    void hideStatusBar();

public slots:
    void onProcessStarted();
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinished(int);
    void onSocketConnection();
    void onSocketData();
    void onSocketError(QLocalSocket::LocalSocketError error);

private:
    QProcess*     process_;
    QLocalServer* server_;
    QLocalSocket* socket_;
};

#endif // CPORTENDPOINT_H
