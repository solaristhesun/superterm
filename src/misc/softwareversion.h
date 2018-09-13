#ifndef SOFTWAREVERSION_H
#define SOFTWAREVERSION_H

#include <QtGlobal>
#include <QLatin1Literal>

class SoftwareVersion
{
public:
    SoftwareVersion(QString versionString);

    QString toString() const;

    quint16 versionMajor() const;
    quint16 versionMinor() const;
    char versionPatch() const;

private:
    quint16 versionMajor_;
    quint8  versionMinor_;
    char    versionPatch_;

};

bool operator>(const SoftwareVersion& lhs, const SoftwareVersion& rhs);

#endif // SOFTWAREVERSION_H
