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
    QTimer::singleShot(0, this, SLOT(enumeratePorts()));
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
    for (const QSerialPortInfo& info : ports)
    {
        m_portsList << CSerialPortInfo(info);
    }

#if defined(Q_OS_LINUX)
    m_portsList << CSerialPortInfo("/dev/COM1", "socat");
#endif

    emit enumerationFinished();

    QTimer::singleShot(1000, this, SLOT(enumeratePorts()));
}

QList<CSerialPortInfo> CPortEnumerator::getAvailablePorts()
{
    QMutexLocker locker(&m_mutex);
    return m_portsList;
}

// EOF <stefan@scheler.com>
