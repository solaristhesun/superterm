#ifndef PORTENUMERATOR_H
#define PORTENUMERATOR_H

#include <QMutex>
#include <QObject>
#include <QThread>

class CSerialPortInfo;

class CPortEnumerator : public QObject
{
    Q_OBJECT
public:
    CPortEnumerator();
    ~CPortEnumerator();

    void startEnumeration();
    void stopEnumeration();

    QList<CSerialPortInfo> getAvailablePorts();

signals:
    void enumerationFinished();

private slots:
    void enumeratePorts();

private:
    QThread                m_workerThread;
    QMutex                 m_mutex;
    QList<CSerialPortInfo> m_portsList;
};

#endif // PORTENUMERATOR_H
