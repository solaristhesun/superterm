#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSerialPort>

extern const QString g_sAppName;
extern const QString g_sAppVersion;
extern const QString g_sAppFullName;
extern const QString g_sAppHomepage;
extern const quint32 g_u32revision;

extern const QMap<QSerialPort::Parity, QString>      g_ParityNameMap;
extern const QMap<QSerialPort::StopBits, QString>    g_StopBitsNameMap;
extern const QMap<QSerialPort::FlowControl, QString> g_FlowControlNameMap;

#endif // GLOBALS_H
