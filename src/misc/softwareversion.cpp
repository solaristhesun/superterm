#include <QRegularExpression>


#include "softwareversion.h"

SoftwareVersion::SoftwareVersion(QString versionString)
{
    QRegularExpression re("/^([0-9]+\\.[0-9]+[a-z]?)$/");
    QRegularExpressionMatch match = re.match(versionString);

    if (match.hasMatch())
    {
        versionMajor_ = match.captured(0).toUShort();
        versionMinor_ = match.captured(1).toUShort();
        versionPatch_ = match.captured(2)[0].toLatin1();
    }
}

QString SoftwareVersion::toString() const
{
    return QString("%1.%2%3").arg(versionMajor_, 4).arg(versionMinor_).arg(versionPatch_);
}

quint16 SoftwareVersion::versionMajor() const
{
    return versionMajor_;
}

quint16 SoftwareVersion::versionMinor() const
{
    return versionMinor_;
}

char SoftwareVersion::versionPatch() const
{
    return versionPatch_;
}

bool operator>(const SoftwareVersion& lhs, const SoftwareVersion& rhs)
{
    if (lhs.versionMajor() > rhs.versionMajor())
    {
        return true;
    }
    else
    {
        if (lhs.versionMinor() > rhs.versionMinor())
        {
            return true;
        }
        else
        {
            if (lhs.versionPatch() > rhs.versionPatch())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
