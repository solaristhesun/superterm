#ifndef PORTENUMERATOR_H
#define PORTENUMERATOR_H

#include <QMutex>
#include <QObject>
#include <QThread>

class SerialPortInfo;

class PortEnumerator : public QObject
{
    Q_OBJECT
public:
    PortEnumerator();
    ~PortEnumerator();

    void startEnumeration();
    void stopEnumeration();

    QList<SerialPortInfo> getAvailablePorts();

signals:
    void enumerationFinished();

private slots:
    void enumeratePorts();

private:
    QThread               m_workerThread;
    QMutex                m_mutex;
    QList<SerialPortInfo> m_portsList;
};

#endif // PORTENUMERATOR_H
