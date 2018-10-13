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

#ifndef PORTAPPLICATION_H
#define PORTAPPLICATION_H

#include <QCoreApplication>
#include <QLocalSocket>

class QSerialPort;
class PortObserver;

class PortApplication : public QCoreApplication
{
    Q_OBJECT
public:
    PortApplication(int& argc, char** argv);

    int exec();

public slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketData();
    void onSerialDataAvailable();
    void onSocketError(QLocalSocket::LocalSocketError error);
    void onPortDisconnected();

private:
    QLocalSocket* socket_;
    QSerialPort*  port_;
    PortObserver* observer_;
    bool          bHasSendReconSignal_;
};

#endif // PORTAPPLICATION_H
