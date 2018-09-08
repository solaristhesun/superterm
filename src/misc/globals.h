#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSerialPort>

class Globals
{
public:
    static const QString ApplicationName;
    static const QString ApplicationFullName;
    static const QString ApplicationVersion;
    static const QString ApplicationWebsite;
    static const quint32 ApplicationRevision;
    static const QMap<QSerialPort::Parity, QString> ParityNameMap;
    static const QMap<QSerialPort::StopBits, QString> StopBitsNameMap;
    static const QMap<QSerialPort::FlowControl, QString> FlowControlNameMap;
};

#endif // GLOBALS_H
