#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QDebug>
#include <iostream>

#include "consolemgr.h"

ConsoleMgr::ConsoleMgr(QObject *parent) :
    QObject(parent)
{
}

void ConsoleMgr::onDataAvailable()
{
    QByteArray data = m_port->readAll();
    std::cout << "received " << data.size() << "bytes" << std::endl;
}
