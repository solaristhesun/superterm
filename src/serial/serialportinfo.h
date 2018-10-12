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

#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include <QMetaType>
#include <QSerialPortInfo>
#include <QString>

class SerialPortInfo
{
public:
    SerialPortInfo();
    SerialPortInfo(QSerialPortInfo portInfo);
    SerialPortInfo(const SerialPortInfo& other);
    ~SerialPortInfo();

    QString description() const;
    QString portName() const;
    QString shortName() const;
    bool    isBusy() const;

    static bool compare(const SerialPortInfo& first, const SerialPortInfo& second);

private:
    QSerialPortInfo portInfo_;
    bool            bBusy_;
};

Q_DECLARE_METATYPE(SerialPortInfo)

#endif // SERIALPORTINFO_H
