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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QtGlobal>

class Message
{
public:
    enum Cmd {
        DataCmd    = 0, // Message contains data
        SigCmd     = 1, // Message contains signal
        UnknownCmd = -1
    };

    enum Signal {
        IsConSig     = 0, // Reconnection mechanism was started
        DoneConSig   = 1, // Reconnection mechanism finished
        CancelConSig = 2, // Reconnection mechanism should be cancelled
        UnknownSig   = -1
    };

    Message();
    ~Message();

    Message(const Cmd& cmd, const QByteArray& payload);
    Message(const Cmd& cmd, const QString& stringData);
    Message(const Cmd& cmd, const Signal& signal);

    void setCmd(const Cmd& u8Cmd);
    Cmd  getCmd() const;

    void       setPayload(const QByteArray& payload);
    QByteArray getPayload() const;

    void    setStringData(const QString& string);
    QString getStringData() const;

    void   setSignal(const Signal& signal);
    Signal getSignal() const;

    bool isCmd(const Cmd& cmd) const;
    int  getSize() const;

private:
    Cmd        m_cmd;
    QByteArray m_payload;
};

QDataStream& operator<<(QDataStream& ds, const Message& obj);
QDataStream& operator>>(QDataStream& ds, Message& obj);

#endif // MESSAGE_H
