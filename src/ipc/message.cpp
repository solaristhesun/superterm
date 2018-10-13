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

#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QDataStream>

#include "ipc/message.h"

Message::Message()
{
    // currently empty
}

Message::~Message()
{
    // currently empty
}

Message::Message(const Cmd& cmd, const QByteArray& payload)
{
    setCmd(cmd);
    setPayload(payload);
}

Message::Message(const Cmd& cmd, const QString& stringData)
{
    setCmd(cmd);
    setStringData(stringData);
}

Message::Message(const Cmd& cmd, const Signal& signal)
{
    setCmd(cmd);
    setSignal(signal);
}

void Message::setCmd(const Cmd& cmd)
{
    cmd_ = cmd;
}

Message::Cmd Message::getCmd() const
{
    return cmd_;
}

void Message::setPayload(const QByteArray& payload)
{
    payload_ = payload;
}

QByteArray Message::getPayload() const
{
    return payload_;
}

void Message::setStringData(const QString& string)
{
    payload_ = string.toLatin1();
}

QString Message::getStringData() const
{
    return QString(payload_);
}

void Message::setSignal(const Signal& signal)
{
    payload_ = QByteArray().append(signal);
}

Message::Signal Message::getSignal() const
{
    return (Signal) payload_.at(0);
}

bool Message::isCmd(const Cmd& cmd) const
{
    return cmd_ == cmd;
}

int Message::getSize() const
{
    /* Total message size is 4 bytes for command + 4 bytes for payload size
     *  + number of bytes in payload */
    return (sizeof(Cmd) + sizeof(qint32) + payload_.size());
}

QDataStream& operator<<(QDataStream& ds, const Message& message)
{
    ds << (qint32)message.getCmd();
    // 4 Bytes size are written too
    ds << message.getPayload();
    return ds;
}

QDataStream& operator>>(QDataStream& ds, Message& message)
{
    qint32     cmd;
    QByteArray payload;

    ds >> cmd;
    // 4 Bytes size are read too
    ds >> payload;

    message = Message((Message::Cmd)cmd, payload);

    return ds;
}
