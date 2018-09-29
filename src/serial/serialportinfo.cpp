#include <QDebug>
#include <QSerialPortInfo>
#include <QRegularExpression>

#include "serial/serialportinfo.h"

SerialPortInfo::SerialPortInfo()
{
    // currently empty
}
SerialPortInfo::SerialPortInfo(QSerialPortInfo portInfo)
    : portInfo_(portInfo)
    , bBusy_(portInfo.isBusy()) // cache busy info, because it is sometimes slow
{
    // currently empty
}

SerialPortInfo::SerialPortInfo(const SerialPortInfo& other)
{
    portInfo_ = other.portInfo_;
    bBusy_ = other.bBusy_;
}

SerialPortInfo::~SerialPortInfo()
{
    // currently nothing
}

QString SerialPortInfo::portName() const
{
    return portInfo_.portName();
}

QString SerialPortInfo::description() const
{
    return portInfo_.description();
}

QString SerialPortInfo::shortName() const
{
#if defined(Q_OS_LINUX)
    return portName().remove("/dev/");
#else
    return portName();
#endif
}

bool SerialPortInfo::isBusy() const
{
    return bBusy_;
}

bool SerialPortInfo::compare(const SerialPortInfo& first, const SerialPortInfo& second)
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
