#ifndef MESSAGECODEC_H
#define MESSAGECODEC_H

#include <QByteArray>

#include "ipc/message.h"

class QByteArray;
class QString;

namespace MessageCodec
{
CMessage decode(QByteArray& message);

QByteArray encodeData(const QByteArray& payload);
QByteArray encodeStringData(const QString& string);
QByteArray encodeSignal(const CMessage::Signal& signal);
QByteArray encode(const CMessage::Cmd& cmd, const QByteArray& payload);
QByteArray encode(const CMessage& message);
}

#endif // MESSAGECODEC_H
