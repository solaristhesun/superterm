#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QDataStream>

#include "ipc/message.h"

Message::Message()
{
}

Message::~Message()
{
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
    m_cmd = cmd;
}

Message::Cmd Message::getCmd() const
{
    return m_cmd;
}

void Message::setPayload(const QByteArray& payload)
{
    m_payload = payload;
}

QByteArray Message::getPayload() const
{
    return m_payload;
}

void Message::setStringData(const QString& string)
{
    m_payload = string.toLatin1();
}

QString Message::getStringData() const
{
    return QString(m_payload);
}

void Message::setSignal(const Signal& signal)
{
    m_payload = QByteArray().append(signal);
}

Message::Signal Message::getSignal() const
{
    return (Signal) m_payload.at(0);
}

bool Message::isCmd(const Cmd& cmd) const
{
    return m_cmd == cmd;
}

int Message::getSize() const
{
    /* Total message size is 4 bytes for command + 4 bytes for payload size
     *  + number of bytes in payload */
    return (sizeof(Cmd) + sizeof(qint32) + m_payload.size());
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
