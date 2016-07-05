#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>
#include <QList>

#include "enumerator/portenumerator.h"
#include "enumerator/serialportinfo.h"

CPortEnumerator::CPortEnumerator():
    m_bActive(false)
{
    qDebug() << "CPortEnumerator::CPortEnumerator()";
    QThread::start();
}

CPortEnumerator::~CPortEnumerator()
{
    qDebug() << "CPortEnumerator::~CPortEnumerator()";
    QThread::exit(0);
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
            //qint64 t1, t2;

            //t1 = QDateTime::currentMSecsSinceEpoch();
            QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
            //t2 = QDateTime::currentMSecsSinceEpoch();

            //qDebug() << "TIME: " << t2 - t1;

            m_mutex.lock();
            m_ports.clear();

            foreach (const QSerialPortInfo &info, ports)
            {
                m_ports << CSerialPortInfo(info);
            }
            m_mutex.unlock();

#if defined(Q_OS_LINUX)
            m_ports << CSerialPortInfo("/dev/COM1", "socat");
#endif
        }

        QThread::sleep(1);
    }
}

QList<CSerialPortInfo> CPortEnumerator::getAvailablePorts(void)
{
    QList<CSerialPortInfo> list;

    m_mutex.lock();
    list = m_ports;
    m_mutex.unlock();

    return list;
}

// EOF <stefan@scheler.com>
