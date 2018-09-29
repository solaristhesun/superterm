#ifndef CPORTENDPOINT_H
#define CPORTENDPOINT_H

#include <QLocalSocket>
#include <QObject>
#include <QProcess>

#include "ipc/message.h"

class QLocalServer;
class QLocalSocket;
class Session;

class PortEndpoint : public QObject
{
    Q_OBJECT
public:
    explicit PortEndpoint(QObject* parent = nullptr);

    void    connectEndpoint(Session* session);
    void    disconnectEndpoint();
    bool    isConnected();
    quint64 writeData(const QByteArray& byteArray);
    quint64 writeSignal(const Message::Signal& signal);
    quint64 write(const QByteArray& byteArray);

signals:
    void readyRead(const Message& message);
    void disconnected(int);
    void connected();
    void showStatusBar();
    void hideStatusBar();

public slots:
    void onProcessStarted();
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinished(int);
    void onSocketConnection();
    void onSocketData();
    void onSocketError(QLocalSocket::LocalSocketError error);

private:
    QProcess*     m_process;
    QLocalServer* m_server;
    QLocalSocket* m_socket;
};

#endif // CPORTENDPOINT_H
