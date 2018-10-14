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

#include <QString>
#include <QSerialPort>
#include <QMap>

#include "misc/globals.h"

const QString Globals::ApplicationName = "superterm";
const SoftwareVersion Globals::ApplicationVersion = SoftwareVersion("2018.10b");
const QString Globals::ApplicationFullName = Globals::ApplicationName + " " + Globals::ApplicationVersion.toString();
const QString Globals::ApplicationWebsite = "https://scheler.com/" + Globals::ApplicationName + "/";
const quint32 Globals::ApplicationRevision = REVISION;

const QMap<QSerialPort::Parity, QString> Globals::ParityNameMap {
    {QSerialPort::NoParity, "None"},
    {QSerialPort::OddParity, "Odd"},
    {QSerialPort::EvenParity, "Even"},
    {QSerialPort::SpaceParity, "Space"},
    {QSerialPort::MarkParity, "Mark"},
};

const QMap<QSerialPort::StopBits, QString> Globals::StopBitsNameMap {
    {QSerialPort::OneStop, "1"},
    {QSerialPort::OneAndHalfStop, "1.5"},
    {QSerialPort::TwoStop, "2"},
};

const QMap<QSerialPort::FlowControl, QString> Globals::FlowControlNameMap {
    {QSerialPort::NoFlowControl, "None"},
    {QSerialPort::SoftwareControl, "Xon/Xoff"},
    {QSerialPort::HardwareControl, "Hardware"},
};

// EOF <stefan@scheler.com>
