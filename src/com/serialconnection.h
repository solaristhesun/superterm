#ifndef CSERIALCONNECTION_H
#define CSERIALCONNECTION_H

#include "connection.h"

class QObject;
class QString;
class QSerialPort;

class CSerialConnection : public CConnection
{
    Q_OBJECT
    Q_PROPERTY(qint32 m_BaudRate READ getBaudRate WRITE setBaudRate)
    Q_PROPERTY(qint32 m_DataBits READ getDataBits WRITE setDataBits)
    Q_PROPERTY(qint32 m_Parity READ getParity WRITE setParity)
    Q_PROPERTY(qint32 m_StopBits READ getStopBits WRITE setStopBits)
    Q_PROPERTY(qint32 m_FlowControl READ getFlowControl WRITE setFlowControl)

public:
    CSerialConnection();
    explicit CSerialConnection(QSerialPort& port);

    QSerialPort* createSerialPort(void);

    void setBaudRate(qint32 baudRate);
    qint32 getBaudRate(void) const;

    void setDataBits(qint32 dataBits);
    qint32 getDataBits() const;

    void setParity(qint32 parity);
    qint32 getParity() const;

    void setStopBits(qint32 stopBits);
    qint32 getStopBits() const;

    void setFlowControl(qint32 flowControl);
    qint32 getFlowControl() const;

private:
    qint32 m_BaudRate;
    qint32 m_DataBits;
    qint32 m_Parity;
    qint32 m_StopBits;
    qint32 m_FlowControl;
};

#endif // CSERIALCONNECTION_H
