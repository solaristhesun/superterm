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

#include <QSerialPort>
#include <QSerialPortInfo>

#include "portobserver.h"

PortObserver::PortObserver()
    : bActive_(false)
    , port_(nullptr)
{
    QThread::start();
}

PortObserver::~PortObserver()
{
    QThread::exit(0);
}

void PortObserver::setActive(bool bActive)
{
    bActive_ = bActive;
}

void PortObserver::setPort(QSerialPort* port)
{
    port_ = port;
}

void PortObserver::run()
{
    while (true)
    {
        if (bActive_)
        {
            if (port_)
            {
                QSerialPortInfo info(*port_);

                if (!info.isBusy())
                {
                    emit disconnected();
                }
            }
        }

        QThread::sleep(1); // seconds
    }
}
