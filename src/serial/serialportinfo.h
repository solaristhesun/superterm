#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include <QString>

class QSerialPortInfo;

class CSerialPortInfo
{
public:
    CSerialPortInfo(const QString strPortName, const QString strPortDesc);
    CSerialPortInfo(const QSerialPortInfo& info);
    ~CSerialPortInfo();

    QString getPortName() const;
    QString getShortName() const;
    QString getDescription() const;

    static bool compare(const CSerialPortInfo& first, const CSerialPortInfo& second);

private:
    QString m_strPortName;
    QString m_strPortDesc;
};

#endif // SERIALPORTINFO_H
