#include <QtGlobal>
#include <QByteArray>
#include <QString>
#include <QDataStream>

#include "ipc/message.h"
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
