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

#include "ipc/messagecodec.h"

Message MessageCodec::decode(QByteArray& message)
{
    QDataStream ds(message);

    Message decodedMessage;
    ds >> decodedMessage;
    message.remove(0, decodedMessage.getSize());

    return decodedMessage;
}

QByteArray MessageCodec::encodeData(const QByteArray& payload)
{
    return encode(Message(Message::DataCmd, payload));
}

QByteArray MessageCodec::encodeStringData(const QString& string)
{
    return encode(Message(Message::DataCmd, string));
}

QByteArray MessageCodec::encodeSignal(const Message::Signal& signal)
{
    return encode(Message(Message::SigCmd, signal));
}

QByteArray MessageCodec::encode(const Message::Cmd& cmd, const QByteArray& payload)
{
    return encode(Message(cmd, payload));
}

QByteArray MessageCodec::encode(const Message& message)
{
    QByteArray  encodedMessage;
    QDataStream ds(&encodedMessage, QIODevice::WriteOnly);
    ds << message;

    return encodedMessage;
}
