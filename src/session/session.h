#ifndef SESSION_H
#define SESSION_H

#include <QVariantList>

#include "session/serializableobject.h"

class CSession : public CSerializableObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString m_deviceName READ getDeviceName WRITE setDeviceName)
    Q_PROPERTY(QString m_deviceDesc READ getDeviceDesc WRITE setDeviceDesc)
    Q_PROPERTY(quint32 m_u32BaudRate READ getBaudRate WRITE setBaudRate)
    Q_PROPERTY(qint32 m_i32DataBits READ getDataBits WRITE setDataBits)
    Q_PROPERTY(qint32 m_i32Parity READ getParity WRITE setParity)
    Q_PROPERTY(qint32 m_i32StopBits READ getStopBits WRITE setStopBits)
    Q_PROPERTY(qint32 m_i32FlowControl READ getFlowControl WRITE setFlowControl)

    Q_PROPERTY(QVariantList m_highlights READ getHighlights WRITE setHighlights)

    CSession();

    quint32 getBaudRate() const { return m_u32BaudRate; }
    void setBaudRate(quint32 u32BaudRate) { m_u32BaudRate = u32BaudRate; }
    QString getDeviceName() const { return m_deviceName; }
    void setDeviceName(const QString deviceName) { m_deviceName = deviceName; }
    QString getDeviceDesc() const { return m_deviceDesc; }
    void setDeviceDesc(const QString deviceDesc) { m_deviceDesc = deviceDesc; }
    qint32 getDataBits() const { return m_i32DataBits; }
    void setDataBits(qint32 i32DataBits) { m_i32DataBits = i32DataBits; }
    qint32 getParity() const { return m_i32Parity; }
    void setParity(qint32 i32Parity) { m_i32Parity = i32Parity; }
    qint32 getStopBits() const { return m_i32StopBits; }
    void setStopBits(qint32 i32StopBits) { m_i32StopBits = i32StopBits; }
    qint32 getFlowControl() const { return m_i32FlowControl; }
    void setFlowControl(qint32 i32FlowControl) { m_i32FlowControl = i32FlowControl; }

    QVariantList getHighlights() const { return m_highlights; }
    void setHighlights(QVariantList highlights) { m_highlights = highlights; }

    void saveToFile();

private:
    QString m_deviceDesc;
    QString m_deviceName;
    quint32 m_u32BaudRate;
    qint32 m_i32DataBits;
    qint32 m_i32Parity;
    qint32 m_i32StopBits;
    qint32 m_i32FlowControl;

    QVariantList m_highlights;
};

#endif // SESSION_H
