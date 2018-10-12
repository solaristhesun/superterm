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

#ifndef MESSAGECODEC_H
#define MESSAGECODEC_H

#include <QByteArray>

#include "ipc/message.h"

class QByteArray;
class QString;

namespace MessageCodec
{
Message decode(QByteArray& message);

QByteArray encodeData(const QByteArray& payload);
QByteArray encodeStringData(const QString& string);
QByteArray encodeSignal(const Message::Signal& signal);
QByteArray encode(const Message::Cmd& cmd, const QByteArray& payload);
QByteArray encode(const Message& message);
}

#endif // MESSAGECODEC_H
