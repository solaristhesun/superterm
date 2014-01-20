#ifndef CONSOLEMGR_H
#define CONSOLEMGR_H

#include <QObject>

class QSerialPort;

class ConsoleMgr : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleMgr(QObject *parent = 0);

signals:

public slots:
    void onDataAvailable(void);

private:
    QSerialPort *m_port;

};

#endif // CONSOLEMGR_H
