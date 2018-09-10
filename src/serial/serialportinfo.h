#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include <QMetaType>
#include <QString>
#include <QSerialPortInfo>

class CSerialPortInfo
{
public:
    CSerialPortInfo();
    CSerialPortInfo(QSerialPortInfo portInfo);
    CSerialPortInfo(const CSerialPortInfo& other);
    ~CSerialPortInfo();

    QString description() const;
    QString portName() const;
    QString shortName() const;
    bool isBusy() const;

    static bool compare(const CSerialPortInfo& first, const CSerialPortInfo& second);

private:
    QSerialPortInfo portInfo_;
    bool            bBusy_;
};

Q_DECLARE_METATYPE(CSerialPortInfo);

#endif // SERIALPORTINFO_H
