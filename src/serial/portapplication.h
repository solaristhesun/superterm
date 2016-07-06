#ifndef PORTAPPLICATION_H
#define PORTAPPLICATION_H

#include <QCoreApplication>
#include <QLocalSocket>

class QSerialPort;

class CPortApplication : public QCoreApplication
{
    Q_OBJECT
public:
    CPortApplication(int &argc, char **argv);

    void connectSocket(void);

public slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketData();
    void onSerialDataAvailable();
    void onSocketError(QLocalSocket::LocalSocketError error);

private:
    QLocalSocket* m_socket;
    QSerialPort*  m_port;
};

#endif // PORTAPPLICATION_H
