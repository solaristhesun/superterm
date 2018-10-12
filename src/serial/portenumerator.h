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

#ifndef PORTENUMERATOR_H
#define PORTENUMERATOR_H

#include <QMutex>
#include <QObject>
#include <QThread>

class SerialPortInfo;

class PortEnumerator : public QObject
{
    Q_OBJECT
public:
    PortEnumerator();
    ~PortEnumerator();

    void startEnumeration();
    void stopEnumeration();

    QList<SerialPortInfo> getAvailablePorts();

signals:
    void enumerationFinished();

private slots:
    void enumeratePorts();

private:
    QThread               m_workerThread;
    QMutex                m_mutex;
    QList<SerialPortInfo> m_portsList;
};

#endif // PORTENUMERATOR_H
