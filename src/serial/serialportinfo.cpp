#include <QDebug>
#include <QSerialPortInfo>
#include <QRegularExpression>

#include "serial/serialportinfo.h"

CSerialPortInfo::CSerialPortInfo(QSerialPortInfo info)
    : portInfo_(info)
{
    // currently empty
}

CSerialPortInfo::~CSerialPortInfo()
{
    // currently nothing
}

QString CSerialPortInfo::portName() const
{
    return portInfo_.portName();
}

QString CSerialPortInfo::shortName() const
{
#if defined(Q_OS_LINUX)
    return portName().remove("/dev/");
#else
    return portName();
#endif
}

QString CSerialPortInfo::getDescription() const
{
    return portInfo_.description();
}

bool CSerialPortInfo::isBusy() const
{
    return portInfo_.isBusy();
}

bool CSerialPortInfo::compare(const CSerialPortInfo& first, const CSerialPortInfo& second)
{
    QRegularExpression re("^(\\D*)(\\d+)\\D*$");
    QRegularExpressionMatch match1 = re.match(first.shortName());
    QRegularExpressionMatch match2 = re.match(second.shortName());

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
        return first.shortName() < second.shortName();
    }
}

// EOF <stefan@scheler.com>
