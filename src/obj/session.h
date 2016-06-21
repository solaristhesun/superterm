#ifndef SESSION_H
#define SESSION_H

#include "obj/seriazableobject.h"

class CSession : public CSeriazableObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString m_deviceName READ getDeviceName WRITE setDeviceName)
    Q_PROPERTY(QString m_deviceDesc READ getDeviceDesc WRITE setDeviceDesc)
    Q_PROPERTY(quint32 m_u32BaudRate READ getBaudRate WRITE setBaudRate)


    CSession();

    quint32 getBaudRate() const { return m_u32BaudRate; }
    void setBaudRate(quint32 u32BaudRate) { m_u32BaudRate = u32BaudRate; }
    QString getDeviceName() const { return m_deviceName; };
    void setDeviceName(const QString deviceName) { m_deviceName = deviceName; }
    QString getDeviceDesc() const { return m_deviceDesc; };
    void setDeviceDesc(const QString deviceDesc) { m_deviceDesc = deviceDesc; }

    void saveToFile();

private:
    QString m_deviceDesc;
    QString m_deviceName;
    quint32 m_u32BaudRate;
};

#endif // SESSION_H
