#ifndef CONNECTIONBAR_H
#define CONNECTIONBAR_H

#include <QFrame>

namespace Ui
{
class ConnectionBar;
}

class Session;
class PortEnumerator;

class ConnectionBar : public QFrame
{
    Q_OBJECT
public:
    void    loadFromSession(Session* session);
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
    void    setPortEnumerator(PortEnumerator* pe);
    explicit ConnectionBar(QWidget* parent = nullptr);
    ~ConnectionBar();

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
    Ui::ConnectionBar* m_ui;

    void fillComboBoxes();
};

#endif // CONNECTIONBAR_H
