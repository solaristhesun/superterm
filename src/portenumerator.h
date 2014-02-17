#ifndef PORTENUMERATOR_H
#define PORTENUMERATOR_H

#include <QThread>

class QSerialPortInfo;

class CPortEnumerator: public QThread
{
public:
    CPortEnumerator();
    ~CPortEnumerator();

    void startEnumeration(void);
    void stopEnumeration(void);

    QList<QSerialPortInfo> getAvailablePorts(void);

    void run(void);



private:
    QList<QSerialPortInfo> m_listPorts;
    bool                   m_bActive;
};

#endif // PORTENUMERATOR_H
