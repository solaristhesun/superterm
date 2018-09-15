#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSerialPort>

#include "misc/softwareversion.h"

class Globals
{
public:
    static const QString         ApplicationName;
    static const QString         ApplicationFullName;
    static const SoftwareVersion ApplicationVersion;
    static const QString         ApplicationWebsite;
    static const quint32         ApplicationRevision;

    static const QMap<QSerialPort::Parity, QString>      ParityNameMap;
    static const QMap<QSerialPort::StopBits, QString>    StopBitsNameMap;
    static const QMap<QSerialPort::FlowControl, QString> FlowControlNameMap;
};

#endif // GLOBALS_H
