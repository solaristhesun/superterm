#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QList>

#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

CPortEnumerator::CPortEnumerator()
{
    qDebug() << "CPortEnumerator::CPortEnumerator()";
    QObject::moveToThread(&m_workerThread);
}

CPortEnumerator::~CPortEnumerator()
{
    qDebug() << "CPortEnumerator::~CPortEnumerator()";
}

void CPortEnumerator::startEnumeration()
{
    qDebug() << "CPortEnumerator::startEnumeration()";
    QTimer::singleShot(0, this, &CPortEnumerator::enumeratePorts);
    m_workerThread.start();
}

void CPortEnumerator::stopEnumeration()
{
    qDebug() << "CPortEnumerator::stopEnumeration()";
    m_workerThread.exit(0);
}

void CPortEnumerator::enumeratePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QMutexLocker locker(&m_mutex);
    m_portsList.clear();
    for (const QSerialPortInfo& portInfo : ports)
    {
        CSerialPortInfo info(portInfo);
        m_portsList << info;
    }

//#if defined(Q_OS_LINUX) && defined(DEBUG)
    m_portsList << CSerialPortInfo(QSerialPortInfo("/dev/COM1"));
//#endif

    std::sort(m_portsList.begin(), m_portsList.end(), CSerialPortInfo::compare);

    emit enumerationFinished();

    QTimer::singleShot(1000, this, &CPortEnumerator::enumeratePorts);
}

QList<CSerialPortInfo> CPortEnumerator::getAvailablePorts()
{
    QMutexLocker locker(&m_mutex);
    return m_portsList;
}

// EOF <stefan@scheler.com>
