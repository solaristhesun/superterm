#include <QDebug>
#include <QSerialPortInfo>
#include <QRegularExpression>

#include "serial/serialportinfo.h"

CSerialPortInfo::CSerialPortInfo(const QString strPortName, const QString strPortDesc)
    : m_strPortName(strPortName)
    , m_strPortDesc(strPortDesc)
{
    // currently nothing
}

CSerialPortInfo::CSerialPortInfo(const QSerialPortInfo& info)
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

bool CSerialPortInfo::compare(const CSerialPortInfo& first, const CSerialPortInfo& second)
{
    QRegularExpression re("^(\\D*)(\\d+)\\D*$");
    QRegularExpressionMatch match1 = re.match(first.getShortName());
    QRegularExpressionMatch match2 = re.match(second.getShortName());

    // try to natural sort if port names match a pattern like "(name)(number)"
    if (match1.hasMatch() && match2.hasMatch())
    {
        QString portName1 = match1.captured(1);
        QString portName2 = match2.captured(1);
        uint portNumber1 = match1.captured(2).toUInt();
        uint portNumber2 = match2.captured(2).toUInt();

        if (portName1 == portName2)
        {
            return portNumber1 < portNumber2; // natural sort by portnumber
        }
        else
        {
            return portName1 < portName2;
        }
    }
    else
    {
        return first.getShortName() < second.getShortName();
    }
}

// EOF <stefan@scheler.com>
