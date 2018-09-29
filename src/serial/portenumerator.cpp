#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QList>

#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

PortEnumerator::PortEnumerator()
{
    qDebug() << "CPortEnumerator::CPortEnumerator()";
    QObject::moveToThread(&m_workerThread);
}

PortEnumerator::~PortEnumerator()
{
    qDebug() << "CPortEnumerator::~CPortEnumerator()";
}

void PortEnumerator::startEnumeration()
{
    qDebug() << "CPortEnumerator::startEnumeration()";
    QTimer::singleShot(0, this, &PortEnumerator::enumeratePorts);
    m_workerThread.start();
}

void PortEnumerator::stopEnumeration()
{
    qDebug() << "CPortEnumerator::stopEnumeration()";
    m_workerThread.exit(0);
}

void PortEnumerator::enumeratePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QMutexLocker locker(&m_mutex);
    m_portsList.clear();
    for (const QSerialPortInfo& portInfo : ports)
    {
        SerialPortInfo info(portInfo);
        m_portsList << info;
    }

//#if defined(Q_OS_LINUX) && defined(DEBUG)
    m_portsList << SerialPortInfo(QSerialPortInfo("/dev/COM1"));
//#endif

    std::sort(m_portsList.begin(), m_portsList.end(), SerialPortInfo::compare);

    emit enumerationFinished();

    QTimer::singleShot(1000, this, &PortEnumerator::enumeratePorts);
}

QList<SerialPortInfo> PortEnumerator::getAvailablePorts()
{
    QMutexLocker locker(&m_mutex);
    return m_portsList;
}

// EOF <stefan@scheler.com>
