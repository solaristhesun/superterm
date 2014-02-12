#include <QSerialPortInfo>
#include <QDebug>

#include "portenumerator.h"

CPortEnumerator::CPortEnumerator():
    m_bActive(false)
{
    start();
}

CPortEnumerator::~CPortEnumerator()
{
    exit(0);
}

void CPortEnumerator::startEnumeration(void)
{
    m_bActive = true;
}

void CPortEnumerator::stopEnumeration(void)
{
    m_bActive = false;
}

void CPortEnumerator::run(void)
{
    while (true)
    {
        if (m_bActive)
        {
            m_listPorts = QSerialPortInfo::availablePorts();
        }

        sleep(1);
    }
}

QList<QSerialPortInfo> CPortEnumerator::getAvailablePorts(void)
{
    return m_listPorts;
}

// EOF <stefan@scheler.com>
