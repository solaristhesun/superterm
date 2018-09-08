#include <QString>
#include <QSerialPort>
#include <QMap>

#include "misc/globals.h"

const QString Globals::ApplicationName = "superterm";
const QString Globals::ApplicationVersion = "2018.8";
const QString Globals::ApplicationFullName = Globals::ApplicationName + " " + Globals::ApplicationVersion;
const QString Globals::ApplicationWebsite = "https://scheler.com/" + Globals::ApplicationName + "/";
const quint32 Globals::ApplicationRevision = REVISION;

const QMap<QSerialPort::Parity, QString> Globals::ParityNameMap {
    {QSerialPort::NoParity, "None"},
    {QSerialPort::OddParity, "Odd"},
    {QSerialPort::EvenParity, "Even"},
    {QSerialPort::SpaceParity, "Space"},
    {QSerialPort::MarkParity, "Mark"},
};

const QMap<QSerialPort::StopBits, QString> Globals::StopBitsNameMap {
    {QSerialPort::OneStop, "1"},
    {QSerialPort::OneAndHalfStop, "1.5"},
    {QSerialPort::TwoStop, "2"},
};

const QMap<QSerialPort::FlowControl, QString> Globals::FlowControlNameMap {
    {QSerialPort::NoFlowControl, "None"},
    {QSerialPort::SoftwareControl, "Xon/Xoff"},
    {QSerialPort::HardwareControl, "Hardware"},
};

// EOF <stefan@scheler.com>
