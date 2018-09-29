#ifndef PORTSCOMBOBOX_H
#define PORTSCOMBOBOX_H

#include <QComboBox>

#include "serial/serialportinfo.h"

class PortEnumerator;

class PortsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit PortsComboBox(QWidget* parent = nullptr);
    void showPopup();

    void setPortEnumerator(PortEnumerator* pe);
    void setPort(QString port);

    SerialPortInfo currentPortInfo() const;
    void           setCurrentDeviceName(const QString& deviceName);

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private slots:
    void onEnumerationFinished();

private:
    PortEnumerator* m_pe;
    QString         m_portToBeSet;

    void refresh();
};

#endif // PORTSCOMBOBOX_H
