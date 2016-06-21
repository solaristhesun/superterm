#ifndef CPORTENDPOINT_H
#define CPORTENDPOINT_H

#include <QObject>
#include <QProcess>
#include <QLocalSocket>

class QLocalServer;
class QLocalSocket;

class CPortEndpoint : public QObject
{
    Q_OBJECT
public:
    explicit CPortEndpoint(QObject *parent = 0);

    void setBaudRate(const quint32 u32BaudRate);
    void connectEndpoint(const QString& sDeviceName);
    void disconnectEndpoint();
    bool isConnected();
    QByteArray readAll();
    quint64 write(const QByteArray &byteArray);

signals:
    void readyRead();
    void disconnected();
    void connected();

public slots:
    void onProcessStarted();
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinished(int);
    void onSocketConnection();
    void onSocketData();
    void onSocketError(QLocalSocket::LocalSocketError error);

private:
    QProcess*          m_process;
    QLocalServer*      m_server;
    QLocalSocket*      m_socket;
    quint32            m_u32BaudRate;
};

#endif // CPORTENDPOINT_H
