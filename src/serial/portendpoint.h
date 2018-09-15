#ifndef CPORTENDPOINT_H
#define CPORTENDPOINT_H

#include <QLocalSocket>
#include <QObject>
#include <QProcess>

#include "ipc/message.h"

class QLocalServer;
class QLocalSocket;
class CSession;

class CPortEndpoint : public QObject
{
    Q_OBJECT
public:
    explicit CPortEndpoint(QObject* parent = nullptr);

    void    connectEndpoint(CSession* session);
    void    disconnectEndpoint();
    bool    isConnected();
    quint64 writeData(const QByteArray& byteArray);
    quint64 writeSignal(const CMessage::Signal& signal);
    quint64 write(const QByteArray& byteArray);

signals:
    void readyRead(const CMessage& message);
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
