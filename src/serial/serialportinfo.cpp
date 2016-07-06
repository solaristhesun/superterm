#include <QSerialPortInfo>

#include "serial/serialportinfo.h"

CSerialPortInfo::CSerialPortInfo(const QString strPortName, const QString strPortDesc)
    : m_strPortName(strPortName)
    , m_strPortDesc(strPortDesc)
{
    // currently nothing
}

CSerialPortInfo::CSerialPortInfo(const QSerialPortInfo &info)
{
    m_strPortName = info.portName();
    m_strPortDesc = info.description();
}

CSerialPortInfo::~CSerialPortInfo()
{
    // currently nothing
}

QString CSerialPortInfo::getPortName() const
{
    return m_strPortName;
}

QString CSerialPortInfo::getShortName() const
{
#if defined(Q_OS_LINUX)
    QString portName(m_strPortName);
    return portName.remove("/dev/");
#else
    return getPortName();
#endif
}

QString CSerialPortInfo::getDescription() const
{
    return m_strPortDesc;
}

// EOF <stefan@scheler.com>
