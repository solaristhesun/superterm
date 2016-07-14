#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QDataStream>

#include "ipc/message.h"

CMessage::CMessage()
{
}

CMessage::~CMessage()
{
}

CMessage::CMessage(const Cmd& cmd, const QByteArray& payload)
{
    setCmd(cmd);
    setPayload(payload);
}

CMessage::CMessage(const Cmd& cmd, const QString& stringData)
{
    setCmd(cmd);
    setStringData(stringData);
}

CMessage::CMessage(const Cmd& cmd, const Signal& signal)
{
    setCmd(cmd);
    setSignal(signal);
}

void CMessage::setCmd(const Cmd& cmd)
{
    m_cmd = cmd;
}

CMessage::Cmd CMessage::getCmd() const
{
    return m_cmd;
}

void CMessage::setPayload(const QByteArray& payload)
{
    m_payload = payload;
}

QByteArray CMessage::getPayload() const
{
    return m_payload;
}

void CMessage::setStringData(const QString& string)
{
    m_payload = string.toLatin1();
}

QString CMessage::getStringData() const
{
    return QString(m_payload);
}

void CMessage::setSignal(const Signal& signal)
{
    m_payload = QByteArray().append(signal);
}

CMessage::Signal CMessage::getSignal() const
{
    return (Signal) m_payload.at(0);
}

bool CMessage::isCmd(const Cmd& cmd) const
{
    return m_cmd == cmd;
}

int CMessage::getSize() const
{
    /* Total message size is 4 bytes for command + 4 bytes for payload size
     *  + number of bytes in payload */
    return (sizeof(Cmd) + sizeof(qint32) + m_payload.size());
}

QDataStream& operator<<(QDataStream& ds, const CMessage& message)
{
    ds << (qint32)message.getCmd();
    // 4 Bytes size are written too
    ds << message.getPayload();
    return ds;
}

QDataStream& operator>>(QDataStream& ds, CMessage& message)
{
    qint32     cmd;
    QByteArray payload;

    ds >> cmd;
    // 4 Bytes size are read too
    ds >> payload;

    message = CMessage((CMessage::Cmd)cmd, payload);

    return ds;
}
