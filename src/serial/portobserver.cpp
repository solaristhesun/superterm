#include <QSerialPort>
#include <QSerialPortInfo>

#include "portobserver.h"

PortObserver::PortObserver()
    : m_bActive(false)
    , m_port(nullptr)
{
    QThread::start();
}

PortObserver::~PortObserver()
{
    QThread::exit(0);
}

void PortObserver::setActive(bool bActive)
{
    m_bActive = bActive;
}

void PortObserver::setPort(QSerialPort* port)
{
    m_port = port;
}

void PortObserver::run()
{
    while (true)
    {
        if (m_bActive)
        {
            if (m_port)
            {
                QSerialPortInfo info(*m_port);

                if (!info.isBusy())
                {
                    emit disconnected();
                }
            }
        }

        QThread::sleep(1); // seconds
    }
}
