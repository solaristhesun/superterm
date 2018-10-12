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

#ifndef PORTOBSERVER_H
#define PORTOBSERVER_H

#include <QThread>

class QSerialPort;

class PortObserver : public QThread
{
    Q_OBJECT

public:
    PortObserver();
    ~PortObserver();

    void setActive(bool bActive);
    void setPort(QSerialPort* port);

    void run();

signals:

    void disconnected();

private:
    bool         m_bActive;
    QSerialPort* m_port;
};

#endif // PORTOBSERVER_H
