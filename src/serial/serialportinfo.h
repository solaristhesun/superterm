#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include <QString>
#include <QSerialPortInfo>

class CSerialPortInfo
{
public:
    CSerialPortInfo(QSerialPortInfo info);
    ~CSerialPortInfo();

    QString portName() const;
    QString shortName() const;
    QString getDescription() const;
    bool isBusy() const;

    static bool compare(const CSerialPortInfo& first, const CSerialPortInfo& second);

private:
    QSerialPortInfo portInfo_;
    QString m_strPortName;
    QString m_strPortDesc;
};

#endif // SERIALPORTINFO_H
