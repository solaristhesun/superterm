#ifndef PORTENUMERATOR_H
#define PORTENUMERATOR_H

#include <QThread>
#include <QMutex>

class CSerialPortInfo;

class CPortEnumerator: public QThread
{
public:
    CPortEnumerator();
    ~CPortEnumerator();

    void startEnumeration(void);
    void stopEnumeration(void);

    QList<CSerialPortInfo> getAvailablePorts(void);

    void run(void);

private:
    QMutex                 m_mutex;
    QList<CSerialPortInfo> m_ports;
    bool                   m_bActive;
};

#endif // PORTENUMERATOR_H
