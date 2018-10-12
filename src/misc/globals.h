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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSerialPort>

#include "misc/softwareversion.h"

class Globals
{
public:
    static const QString         ApplicationName;
    static const QString         ApplicationFullName;
    static const SoftwareVersion ApplicationVersion;
    static const QString         ApplicationWebsite;
    static const quint32         ApplicationRevision;

    static const QMap<QSerialPort::Parity, QString>      ParityNameMap;
    static const QMap<QSerialPort::StopBits, QString>    StopBitsNameMap;
    static const QMap<QSerialPort::FlowControl, QString> FlowControlNameMap;
};

#endif // GLOBALS_H
