#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>
#include <QList>

#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

CPortEnumerator::CPortEnumerator()
    : m_bActive(false)
{
    qDebug() << "CPortEnumerator::CPortEnumerator()";
    QThread::start();
}

CPortEnumerator::~CPortEnumerator()
{
    qDebug() << "CPortEnumerator::~CPortEnumerator()";
    QThread::exit(0);
}

void CPortEnumerator::startEnumeration()
{
    m_bActive = true;
}

void CPortEnumerator::stopEnumeration()
{
    m_bActive = false;
}

void CPortEnumerator::run()
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

            for (const QSerialPortInfo& info: ports)
            {
                m_ports << CSerialPortInfo(info);
            }
            m_mutex.unlock();

#if defined(Q_OS_LINUX)
            m_ports << CSerialPortInfo("/dev/COM1", "socat");
#endif
        }

        QThread::sleep(1); // seconds
    }
}

QList<CSerialPortInfo> CPortEnumerator::getAvailablePorts()
{
    QList<CSerialPortInfo> list;

    m_mutex.lock();
    list = m_ports;
    m_mutex.unlock();

    return list;
}

// EOF <stefan@scheler.com>
