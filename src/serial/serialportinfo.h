#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include <QMetaType>
#include <QSerialPortInfo>
#include <QString>

class SerialPortInfo
{
public:
    SerialPortInfo();
    SerialPortInfo(QSerialPortInfo portInfo);
    SerialPortInfo(const SerialPortInfo& other);
    ~SerialPortInfo();

    QString description() const;
    QString portName() const;
    QString shortName() const;
    bool    isBusy() const;

    static bool compare(const SerialPortInfo& first, const SerialPortInfo& second);

private:
    QSerialPortInfo portInfo_;
    bool            bBusy_;
};

Q_DECLARE_METATYPE(SerialPortInfo)

#endif // SERIALPORTINFO_H
