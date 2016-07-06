#include <QCoreApplication>
#include <QLocalSocket>
#include <QDebug>
#include <QTimer>
#include <QSerialPort>

#include "serial/portapplication.h"

#if 0
void dumpDCB(const char *szFileName)
{
    printf("using port %s\n", szFileName);

#if defined(Q_OS_WIN)
    HANDLE h = CreateFileA(szFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, 0);

    DCB dcb;

    GetCommState(h, &dcb);

    printf("dcb.DCBlength:         0x%08x\n", dcb.DCBlength);
    printf("dcb.BaudRate:          0x%08x (%lu)\n", dcb.BaudRate, dcb.BaudRate);
    printf("dcb.fBinary:           0x%08x\n", dcb.fBinary);
    printf("dcb.fParity:           0x%08x\n", dcb.fParity);
    printf("dcb.fOutxCtsFlow:      0x%08x\n", dcb.fOutxCtsFlow);
    printf("dcb.fOutxDsrFlow:      0x%08x\n", dcb.fOutxDsrFlow);
    printf("dcb.fDtrControl:       0x%08x\n", dcb.fDtrControl);
    printf("dcb.fDsrSensitivity:   0x%08x\n", dcb.fDsrSensitivity);
    printf("dcb.fTXContinueOnXoff: 0x%08x\n", dcb.fTXContinueOnXoff);
    printf("dcb.fOutX:             0x%08x\n", dcb.fOutX);
    printf("dcb.fInX:              0x%08x\n", dcb.fInX);
    printf("dcb.fErrorChar:        0x%08x\n", dcb.fErrorChar);
    printf("dcb.fNull:             0x%08x\n", dcb.fNull);
    printf("dcb.fRtsControl:       0x%08x\n", dcb.fRtsControl);
    printf("dcb.fAbortOnError:     0x%08x\n", dcb.fAbortOnError);
    printf("dcb.XonLim:            0x%04x\n", dcb.XonLim);
    printf("dcb.XoffLim:           0x%04x\n", dcb.XoffLim);
    printf("dcb.ByteSize:          0x%02x\n", dcb.ByteSize);
    printf("dcb.Parity:            0x%02x\n", dcb.Parity);
    printf("dcb.StopBits:          0x%02x\n", dcb.StopBits);

    printf("dcb.XonChar:           0x%02x\n", dcb.XonChar);
    printf("dcb.XoffChar:          0x%02x\n", dcb.XoffChar);
    printf("dcb.ErrorChar:         0x%02x\n", dcb.ErrorChar);
    printf("dcb.EofChar:           0x%02x\n", dcb.EofChar);
    printf("dcb.EvtChar:           0x%02x\n", dcb.EvtChar);

    CloseHandle(h);
#endif
}
#endif

CPortApplication::CPortApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
    , m_socket(new QLocalSocket(this))
    , m_port(NULL)
{
    // currently nothing
}

void CPortApplication::connectSocket(void)
{
    QString portName = arguments().at(1);
    quint32 baudRate = arguments().at(2).toUInt();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(arguments().at(3).toInt());
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(arguments().at(4).toInt());
    QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(arguments().at(5).toInt());
    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(arguments().at(6).toInt());


    connect(m_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onSocketData()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onSocketError(QLocalSocket::LocalSocketError)));

    m_port = new QSerialPort(portName);

    if (m_port->open(QIODevice::ReadWrite))
    {
        QTextStream(stdout) << "open" << portName << endl;
        m_port->setBaudRate(baudRate);
        m_port->setFlowControl(flowControl);
        m_port->setParity(parity);
        m_port->setStopBits(stopBits);
        m_port->setDataBits(dataBits);
    }
    else
    {
        QTextStream(stdout) << "error:" << m_port->errorString();
    }

    connect(m_port, SIGNAL(readyRead()), this, SLOT(onSerialDataAvailable()));

    m_socket->abort();
    m_socket->connectToServer("serial:" + portName.replace("/", "_"));

    QTextStream(stdout) << "started" << portName << endl;
}

void CPortApplication::onSerialDataAvailable()
{
    m_socket->write(m_port->readAll());
}


void CPortApplication::onSocketData()
{
    m_port->write(m_socket->readAll());
}

void CPortApplication::onSocketDisconnected()
{
    QTextStream(stdout) << "disconnected" << endl;
    QCoreApplication::quit();
}

void CPortApplication::onSocketConnected(void)
{
    QTextStream(stdout) << "connected" << endl;
}

void CPortApplication::onSocketError(QLocalSocket::LocalSocketError error)
{
    QTextStream(stdout) << error << endl;
    QCoreApplication::quit();
}

// EOF <stefan@scheler.com>
