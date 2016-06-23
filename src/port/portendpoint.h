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
    void setDataBits(const qint32 i32DataBits);
    void setParity(const qint32 i32Parity);
    void setStopBits(const qint32 i32StopBits);
    void setFlowControl(const qint32 i32FlowControl);

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
    qint32             m_i32DataBits;
    qint32             m_i32Parity;
    qint32             m_i32StopBits;
    qint32             m_i32FlowControl;
};

#endif // CPORTENDPOINT_H
