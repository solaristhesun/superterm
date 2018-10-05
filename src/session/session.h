#ifndef SESSION_H
#define SESSION_H

#include <QVariantList>

#include "session/serializableobject.h"

class Session : public SerializableObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString m_deviceName READ getDeviceName WRITE setDeviceName)
    Q_PROPERTY(QString m_deviceDesc READ getDeviceDesc WRITE setDeviceDesc)
    Q_PROPERTY(quint32 m_u32BaudRate READ getBaudRate WRITE setBaudRate)
    Q_PROPERTY(qint32 m_i32DataBits READ getDataBits WRITE setDataBits)
    Q_PROPERTY(qint32 m_i32Parity READ getParity WRITE setParity)
    Q_PROPERTY(qint32 m_i32StopBits READ getStopBits WRITE setStopBits)
    Q_PROPERTY(qint32 m_i32FlowControl READ getFlowControl WRITE setFlowControl)
    Q_PROPERTY(QString m_tabLabel READ getTabLabel WRITE setTabLabel)
    Q_PROPERTY(bool m_bUseTimeStamps READ getUseTimeStamps WRITE setUseTimeStamps)
    Q_PROPERTY(QString logFileName_ READ getLogFileName WRITE setLogFileName)

    Q_PROPERTY(QVariantList m_highlights READ getHighlights WRITE setHighlights)

    Session();
    ~Session();
    static Session*    createSessionFromFile(const QString& fileName);
    static void        removeFiles();
    static QStringList getSessionList();

    quint32 getBaudRate() const
    {
        return m_u32BaudRate;
    }
    void setBaudRate(quint32 u32BaudRate)
    {
        m_u32BaudRate = u32BaudRate;
    }
    QString getDeviceName() const
    {
        return m_deviceName;
    }
    QString getTabLabel() const
    {
        return m_tabLabel;
    }
    void setDeviceName(const QString deviceName)
    {
        m_deviceName = deviceName;
    }
    void setTabLabel(const QString tabLabel)
    {
        m_tabLabel = tabLabel;
    }
    QString getDeviceDesc() const
    {
        return m_deviceDesc;
    }
    void setDeviceDesc(const QString deviceDesc)
    {
        m_deviceDesc = deviceDesc;
    }
    qint32 getDataBits() const
    {
        return m_i32DataBits;
    }
    void setDataBits(qint32 i32DataBits)
    {
        m_i32DataBits = i32DataBits;
    }
    qint32 getParity() const
    {
        return m_i32Parity;
    }
    void setParity(qint32 i32Parity)
    {
        m_i32Parity = i32Parity;
    }
    qint32 getStopBits() const
    {
        return m_i32StopBits;
    }
    void setStopBits(qint32 i32StopBits)
    {
        m_i32StopBits = i32StopBits;
    }
    qint32 getFlowControl() const
    {
        return m_i32FlowControl;
    }
    void setFlowControl(qint32 i32FlowControl)
    {
        m_i32FlowControl = i32FlowControl;
    }

    bool getUseTimeStamps() const
    {
        return m_bUseTimeStamps;
    }

    void setUseTimeStamps(bool bUseTimeStamps)
    {
        m_bUseTimeStamps = bUseTimeStamps;
    }

    QVariantList getHighlights() const
    {
        return m_highlights;
    }
    void setHighlights(QVariantList highlights)
    {
        m_highlights = highlights;
    }

    void setLogFileName(QString logFileName)
    {
        logFileName_ = logFileName;
    }
    QString getLogFileName() const
    {
        return logFileName_;
    }

    void saveToFile();

    void setPortConnected(bool bPortConnected);
    bool isPortConnected() const;

private:
    QString m_deviceDesc;
    QString m_deviceName;
    QString m_tabLabel;
    quint32 m_u32BaudRate;
    qint32  m_i32DataBits;
    qint32  m_i32Parity;
    qint32  m_i32StopBits;
    qint32  m_i32FlowControl;
    bool    m_bUseTimeStamps;
    QString logFileName_;

    QVariantList m_highlights;

    bool bPortConnected_;
};

QDebug operator<<(QDebug dbg, const Session& s);

#endif // SESSION_H
