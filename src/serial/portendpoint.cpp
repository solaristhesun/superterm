#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QCoreApplication>

#include "ipc/messagecodec.h"
#include "serial/portendpoint.h"
#include "session/session.h"

CPortEndpoint::CPortEndpoint(QObject* parent)
    : QObject(parent)
    , m_process(NULL)
    , m_server(NULL)
    , m_socket(NULL)
{
    // currently nothing
}

void CPortEndpoint::onProcessFinished(int retval)
{
    qDebug() << "[slot] onProcessFinished" << retval;

    disconnectEndpoint();
    emit disconnected(retval);
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
    QByteArray array = m_socket->readAll();
    while (array.size() > 0)
    {
        emit readyRead(MessageCodec::decode(array));
    }
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
}

void CPortEndpoint::connectEndpoint(CSession* session)
{
    m_process = new QProcess(this);
    m_server  = new QLocalServer(this);

    connect(m_process, SIGNAL(started()), this, SLOT(onProcessStarted()));
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    connect(m_process, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onSocketConnection()));

    qDebug() << "LISTEN" << "serial:" + session->getDeviceName();

    QString socketName = session->getDeviceName();
    socketName = socketName.replace("/", "_");

    if (!m_server->listen("serial:" + socketName))
    {
        qDebug() << "ERROR LISTEN" << m_server->errorString();
    }

    QStringList args;

    args << session->getDeviceName()
         << QString::number(session->getBaudRate())
         << QString::number(session->getDataBits())
         << QString::number(session->getParity())
         << QString::number(session->getStopBits())
         << QString::number(session->getFlowControl());

    m_process->setProcessChannelMode(QProcess::ForwardedChannels);
    m_process->start(QCoreApplication::applicationFilePath(), args);
}

bool CPortEndpoint::isConnected()
{
    return m_socket != NULL;
}

// EOF <stefan@scheler.com>
