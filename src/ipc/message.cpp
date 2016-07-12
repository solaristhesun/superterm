#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QDataStream>
#include <QTextStream>

#include "ipc/message.h"

CMessage::CMessage()
{
}

CMessage::~CMessage()
{
}

CMessage::CMessage(const QByteArray& byteArray)
{
    setCmd((Cmd)byteArray.at(0));
    setPayload(byteArray.mid(1));
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
