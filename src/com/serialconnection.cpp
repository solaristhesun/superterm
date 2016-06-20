#include <QSerialPort>
#include "serialconnection.h"

CSerialConnection::CSerialConnection() :
    CConnection()
{

}

CSerialConnection::CSerialConnection(QSerialPort &port) :
    CConnection()
{
    setName(port.portName());
    setBaudRate(port.baudRate());
    setDataBits(port.dataBits());
    setParity(port.parity());
    setStopBits(port.stopBits());
    setFlowControl(port.flowControl());
}

QSerialPort* CSerialConnection::createSerialPort(void)
{
    QSerialPort* port = new QSerialPort(getName());
    port->setBaudRate(getBaudRate());
    port->setDataBits((QSerialPort::DataBits)getDataBits());
    port->setParity((QSerialPort::Parity)getParity());
    port->setStopBits((QSerialPort::StopBits)getStopBits());
    port->setFlowControl((QSerialPort::FlowControl)getFlowControl());

    return port;
}

void CSerialConnection::setBaudRate(qint32 baudRate)
{
    m_BaudRate = baudRate;
}

qint32 CSerialConnection::getBaudRate() const
{
    return m_BaudRate;
}

void CSerialConnection::setDataBits(qint32 dataBits)
{
    m_DataBits = dataBits;
}

qint32 CSerialConnection::getDataBits() const
{
    return m_DataBits;
}

void CSerialConnection::setParity(qint32 parity)
{
    m_Parity = parity;
}

qint32 CSerialConnection::getParity() const
{
    return m_Parity;
}

void CSerialConnection::setStopBits(qint32 stopBits)
{
    m_StopBits = stopBits;
}

qint32 CSerialConnection::getStopBits() const
{
    return m_StopBits;
}

void CSerialConnection::setFlowControl(qint32 flowControl)
{
    m_FlowControl = flowControl;
}

qint32 CSerialConnection::getFlowControl() const
{
    return m_FlowControl;
}
