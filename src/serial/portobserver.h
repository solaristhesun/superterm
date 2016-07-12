#ifndef PORTOBSERVER_H
#define PORTOBSERVER_H

#include <QThread>

class QSerialPort;

class CPortObserver : public QThread
{
    Q_OBJECT

public:
    CPortObserver();
    ~CPortObserver();

    void setActive(bool bActive);
    void setPort(QSerialPort* port);

    void run();

signals:

    void disconnected();

private:
    bool         m_bActive;
    QSerialPort* m_port;
};

#endif // PORTOBSERVER_H
