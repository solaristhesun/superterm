#include <QtGlobal>
#include <QByteArray>
#include <QString>

#include "ipc/message.h"
#include "ipc/messagecodec.h"

CMessage MessageCodec::decode(const QByteArray& message)
{
    CMessage::Cmd cmd = (CMessage::Cmd) message.at(0);
    QByteArray    payload = message.mid(1);

    return CMessage(cmd, payload);
}

QByteArray MessageCodec::encode(const CMessage& message)
{
    return encode(message.getCmd(), message.getPayload());
}

QByteArray MessageCodec::encodeData(const QByteArray& payload)
{
    return encode(CMessage::DataCmd, payload);
}

QByteArray MessageCodec::encodeStringData(const QString& string)
{
    return encode(CMessage::DataCmd, string.toLatin1());
}

QByteArray MessageCodec::encodeSignal(const CMessage::Signal& signal)
{
    return encode(CMessage::SigCmd, QByteArray().append(signal));
}

QByteArray MessageCodec::encode(const CMessage::Cmd& cmd, const QByteArray& payload)
{
    QByteArray encodedMessage;

    encodedMessage.append((qint8)cmd);
    encodedMessage.append(payload);

    return encodedMessage;
}
