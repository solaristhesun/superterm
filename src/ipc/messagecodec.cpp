#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QDataStream>

#include "ipc/message.h"
#include "ipc/messagecodec.h"

CMessage MessageCodec::decode(QByteArray& message)
{
    QDataStream ds(message);

    CMessage decodedMessage;
    ds >> decodedMessage;
    message.remove(0, decodedMessage.getSize());

    return decodedMessage;
}

QByteArray MessageCodec::encodeData(const QByteArray& payload)
{
    return encode(CMessage(CMessage::DataCmd, payload));
}

QByteArray MessageCodec::encodeStringData(const QString& string)
{
    return encode(CMessage(CMessage::DataCmd, string));
}

QByteArray MessageCodec::encodeSignal(const CMessage::Signal& signal)
{
    return encode(CMessage(CMessage::SigCmd, signal));
}

QByteArray MessageCodec::encode(const CMessage::Cmd& cmd, const QByteArray& payload)
{
    return encode(CMessage(cmd, payload));
}

QByteArray MessageCodec::encode(const CMessage& message)
{
    QByteArray  encodedMessage;
    QDataStream ds(&encodedMessage, QIODevice::WriteOnly);
    ds << message;

    return encodedMessage;
}
