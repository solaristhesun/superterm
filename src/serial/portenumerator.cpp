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

#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QList>

#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

PortEnumerator::PortEnumerator()
{
    qDebug() << "PortEnumerator::PortEnumerator()";
    QObject::moveToThread(&workerThread_);
}

PortEnumerator::~PortEnumerator()
{
    qDebug() << "PortEnumerator::~PortEnumerator()";
}

void PortEnumerator::startEnumeration()
{
    qDebug() << "PortEnumerator::startEnumeration()";
    QTimer::singleShot(0, this, &PortEnumerator::enumeratePorts);
    workerThread_.start();
}

void PortEnumerator::stopEnumeration()
{
    qDebug() << "PortEnumerator::stopEnumeration()";
    workerThread_.exit(0);
}

void PortEnumerator::enumeratePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QMutexLocker locker(&mutex_);
    portsList_.clear();
    for (const QSerialPortInfo& portInfo : ports)
    {
        SerialPortInfo info(portInfo);
        portsList_ << info;
    }
    std::sort(portsList_.begin(), portsList_.end(), SerialPortInfo::compare);

    emit enumerationFinished();

    QTimer::singleShot(1000, this, &PortEnumerator::enumeratePorts);
}

QList<SerialPortInfo> PortEnumerator::getAvailablePorts()
{
    QMutexLocker locker(&mutex_);
    return portsList_;
}

// EOF <stefan@scheler.com>
