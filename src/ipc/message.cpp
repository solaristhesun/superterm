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
    return (sizeof(Cmd) + sizeof(qint8) + m_payload.size());
}

QDataStream& operator<<(QDataStream& ds, const CMessage& message)
{
    ds << (qint8)message.getCmd();
    ds << message.getPayload();
    return ds;
}

QDataStream& operator>>(QDataStream& ds, CMessage& message)
{
    qint8      cmd;
    QByteArray payload;

    ds >> cmd;
    ds >> payload;

    message = CMessage((CMessage::Cmd)cmd, payload);

    return ds;
}
