#ifndef SOFTWAREVERSION_H
#define SOFTWAREVERSION_H

#include <QDebug>
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
    bool isValid() const;

    bool operator==(const SoftwareVersion& other);
    bool operator>(const SoftwareVersion& other);
    bool operator<(const SoftwareVersion& other);
    bool operator<=(const SoftwareVersion& other);
    bool operator>=(const SoftwareVersion& other);

private:
    quint16 versionMajor_;
    quint8  versionMinor_;
    char    versionPatch_;

};

QDebug operator<<(QDebug dbg, const SoftwareVersion& version);

#endif // SOFTWAREVERSION_H
