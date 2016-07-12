#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>

#include "ipc/messagecodec.h"
#include "serial/portendpoint.h"

CPortEndpoint::CPortEndpoint(QObject* parent)
    : QObject(parent)
    , m_process(NULL)
    , m_server(NULL)
    , m_socket(NULL)
    , m_u32BaudRate(115200)
{
    // currently nothing
}

void CPortEndpoint::onProcessFinished(int)
{
    qDebug() << "[slot] onProcessFinished";
    disconnectEndpoint();
}

void CPortEndpoint::onProcessStarted()
{
    qDebug() << "[slot] onProcessStarted";
}

void CPortEndpoint::onProcessError(QProcess::ProcessError error)
{
    qDebug() << "[slot] onProcessError:" << error;
}

void CPortEndpoint::onSocketConnection()
{
    qDebug() << "[slot] onSocketConnection";

    m_socket = m_server->nextPendingConnection();

    if (m_socket)
    {
        connect(m_socket, SIGNAL(disconnected()), m_socket, SLOT(deleteLater()));
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(onSocketData()));
        connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onSocketError(QLocalSocket::LocalSocketError)));

        emit connected();
    }
}

void CPortEndpoint::onSocketData()
{
    emit readyRead(MessageCodec::decode(m_socket->readAll()));
}

void CPortEndpoint::onSocketError(QLocalSocket::LocalSocketError error)
{
    qDebug() << "[slot] onSocketError:" << error;
}

quint64 CPortEndpoint::writeData(const QByteArray& byteArray)
{
    return write(MessageCodec::encodeData(byteArray));
}

quint64 CPortEndpoint::writeSignal(const CMessage::Signal& signal)
{
    return write(MessageCodec::encodeSignal(signal));
}

quint64 CPortEndpoint::write(const QByteArray& byteArray)
{
    quint64 u64ret = 0;

    if (m_socket)
    {
        u64ret = m_socket->write(byteArray);
    }

    return u64ret;
}

void CPortEndpoint::disconnectEndpoint()
{
    if (m_socket)
    {
        delete m_socket;
        m_socket = NULL;
    }

    if (m_server)
    {
        delete m_server;
        m_server = NULL;
    }

    if (m_process)
    {
        m_process->deleteLater();
    }

    emit disconnected();
}

void CPortEndpoint::connectEndpoint(const QString& sDeviceName)
{
    m_process = new QProcess(this);
    m_server  = new QLocalServer(this);

    connect(m_process, SIGNAL(started()), this, SLOT(onProcessStarted()));
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    connect(m_process, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onSocketConnection()));

    qDebug() << "LISTEN" << "serial:" + sDeviceName;

    QString socketName = sDeviceName;
    socketName = socketName.replace("/", "_");

    if (!m_server->listen("serial:" + socketName))
    {
        qDebug() << "ERROR LISTEN"<< m_server->errorString();
    }

    QStringList args;
    args << sDeviceName << QString::number(m_u32BaudRate) << QString::number(m_i32DataBits);
    args << QString::number(m_i32Parity) << QString::number(m_i32StopBits) << QString::number(m_i32FlowControl);

    m_process->setProcessChannelMode(QProcess::ForwardedChannels);
    m_process->start(QCoreApplication::applicationFilePath(), args);
}

bool CPortEndpoint::isConnected()
{
    return m_socket != NULL;
}

void CPortEndpoint::setBaudRate(const quint32 u32BaudRate)
{
    m_u32BaudRate = u32BaudRate;
}

void CPortEndpoint::setDataBits(const qint32 i32DataBits)
{
    m_i32DataBits = i32DataBits;
}

void CPortEndpoint::setParity(const qint32 i32Parity)
{
    m_i32Parity = i32Parity;
}

void CPortEndpoint::setStopBits(const qint32 i32StopBits)
{
    m_i32StopBits = i32StopBits;
}

void CPortEndpoint::setFlowControl(const qint32 i32FlowControl)
{
    m_i32FlowControl = i32FlowControl;
}

// EOF <stefan@scheler.com>
