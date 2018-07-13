#include <QString>
#include <QSerialPort>
#include <QMap>

#include "misc/globals.h"

const QString g_sAppName     = "superterm";
const QString g_sAppVersion  = "2018.7";
const QString g_sAppFullName = g_sAppName + " " + g_sAppVersion;
const QString g_sAppHomepage = "https://scheler.com/" + g_sAppName + "/";
const quint32 g_u32revision  = REVISION;

const QMap<QSerialPort::Parity, QString> g_ParityNameMap {
    {QSerialPort::NoParity, "None"},
    {QSerialPort::OddParity, "Odd"},
    {QSerialPort::EvenParity, "Even"},
    {QSerialPort::SpaceParity, "Space"},
    {QSerialPort::MarkParity, "Mark"},
};

const QMap<QSerialPort::StopBits, QString> g_StopBitsNameMap {
    {QSerialPort::OneStop, "1"},
    {QSerialPort::OneAndHalfStop, "1.5"},
    {QSerialPort::TwoStop, "2"},
};

const QMap<QSerialPort::FlowControl, QString> g_FlowControlNameMap {
    {QSerialPort::NoFlowControl, "None"},
    {QSerialPort::SoftwareControl, "Xon/Xoff"},
    {QSerialPort::HardwareControl, "Hardware"},
};

// EOF <stefan@scheler.com>
