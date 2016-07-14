#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtGlobal>
#include <QByteArray>

class CMessage
{
public:

    enum Cmd
    {
        DataCmd    = 0, // Message contains data
        SigCmd     = 1, // Message contains signal
        UnknownCmd = -1
    };

    enum Signal
    {
        IsConSig     = 0, // Reconnection mechanism was started
        DoneConSig   = 1, // Reconnection mechanism finished
        CancelConSig = 2, // Reconnection mechanism should be cancelled
        UnknownSig   = -1
    };

    CMessage();
    ~CMessage();

    CMessage(const Cmd& cmd, const QByteArray& payload);
    CMessage(const Cmd& cmd, const QString& stringData);
    CMessage(const Cmd& cmd, const Signal& signal);

    void setCmd(const Cmd& u8Cmd);
    Cmd getCmd() const;

    void setPayload(const QByteArray& payload);
    QByteArray getPayload() const;

    void setStringData(const QString& string);
    QString getStringData() const;

    void setSignal(const Signal& signal);
    Signal getSignal() const;

    bool isCmd(const Cmd& cmd) const;
    int getSize() const;

private:
    Cmd        m_cmd;
    QByteArray m_payload;
};

QDataStream& operator<<(QDataStream& ds, const CMessage& obj);
QDataStream& operator>>(QDataStream& ds, CMessage& obj);

#endif // MESSAGE_H
