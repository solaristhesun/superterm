#include <QSerialPort>
#include <QSerialPortInfo>

#include "portobserver.h"

CPortObserver::CPortObserver()
    : m_bActive(false)
    , m_port(NULL)
{
    QThread::start();
}

CPortObserver::~CPortObserver()
{
    QThread::exit(0);
}

void CPortObserver::setActive(bool bActive)
{
    m_bActive = bActive;
}

void CPortObserver::setPort(QSerialPort* port)
{
    m_port = port;
}

void CPortObserver::run()
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
