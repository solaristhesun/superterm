#include <QSerialPort>
#include <QSerialPortInfo>
#include <iostream>

#include "consolemgr.h"

ConsoleMgr::ConsoleMgr(QObject *parent) :
    QObject(parent)
{
    QList<QSerialPortInfo> info = QSerialPortInfo::availablePorts();

    std::cout << "hallo" << info.size() << std::endl;

    QList<QSerialPortInfo>::iterator i;
    for(i=info.begin(); i!=info.end(); ++i) {
        std::cout << i->portName().toStdString() << std::endl;
    }

    QSerialPortInfo info2("/dev/pts/16");
    std::cout << info2.description().toStdString() << "DONE" << std::endl;
#if 0
    m_port = new QSerialPort("/dev/pts/16")

    if (m_port->open(QIODevice::ReadWrite))
    {
        std::cout << "PORT OPEN" << std::endl;
    }

    connect(m_port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
#endif
}

void ConsoleMgr::onDataAvailable()
{
    QByteArray data = m_port->readAll();
    std::cout << "received " << data.size() << "bytes" << std::endl;
}
