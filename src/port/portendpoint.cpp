#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>

#include "portendpoint.h"

CPortEndpoint::CPortEndpoint(QObject *parent)
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
    qDebug() << "PROCESS FINISHED";
    disconnectEndpoint();
}

void CPortEndpoint::onStandardErrorData()
{
    qDebug() << "[subprocess] " << m_process->readAllStandardOutput();
}

void CPortEndpoint::onProcessStarted()
{
    qDebug() << "process started\n";
}

void CPortEndpoint::onProcessError(QProcess::ProcessError error)
{
    qDebug() << "error " << error << "\n";
}

void CPortEndpoint::onSocketConnection()
{
    qDebug() << "INCOMING\n";
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
    emit readyRead();
}

void CPortEndpoint::onSocketError(QLocalSocket::LocalSocketError error)
{
    qDebug() << error;
}

QByteArray CPortEndpoint::readAll()
{
    return m_socket->readAll();
}

quint64 CPortEndpoint::write(const QByteArray &byteArray)
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

    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onStandardErrorData()));
    connect(m_process, SIGNAL(started()), this, SLOT(onProcessStarted()));
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    connect(m_process, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onSocketConnection()));

    m_server->listen("serial:" + sDeviceName);

    QStringList args;
    args << sDeviceName << QString::number(m_u32BaudRate);

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

// EOF <stefan@scheler.com>
