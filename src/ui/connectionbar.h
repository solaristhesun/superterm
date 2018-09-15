#ifndef CONNECTIONBAR_H
#define CONNECTIONBAR_H

#include <QFrame>

namespace Ui
{
class CConnectionBar;
}

class CSession;
class CPortEnumerator;

class CConnectionBar : public QFrame
{
    Q_OBJECT
public:
    void    loadFromSession(CSession* session);
    void    onConnected();
    void    onDisconnected();
    QString getDeviceName() const;
    QString getDeviceDesc() const;
    QString getBaudRate() const;
    QString getDataBits() const;
    QString getParity() const;
    QString getStopBits() const;
    QString getFlowControl() const;
    void    setDeviceName(const QString& deviceName);
    void    setBaudRate(const QString& baudRate);
    void    setDataBits(const QString& dataBits);
    void    setParity(const QString& parity);
    void    setStopBits(const QString& stopBits);
    void    setFlowControl(const QString& flowControl);
    void    setPortEnumerator(CPortEnumerator* pe);
    explicit CConnectionBar(QWidget* parent = nullptr);
    ~CConnectionBar();

protected:
    void showEvent(QShowEvent* event);

public slots:
    void onMoreClicked();
    void onComboChanged();

signals:
    void connectClicked();
    void saveClicked();
    void configurationChanged(const QString& config);

private:
    Ui::CConnectionBar* m_ui;

    void fillComboBoxes();
};

#endif // CONNECTIONBAR_H
