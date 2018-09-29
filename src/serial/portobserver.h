#ifndef PORTOBSERVER_H
#define PORTOBSERVER_H

#include <QThread>

class QSerialPort;

class PortObserver : public QThread
{
    Q_OBJECT

public:
    PortObserver();
    ~PortObserver();

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
