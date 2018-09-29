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
