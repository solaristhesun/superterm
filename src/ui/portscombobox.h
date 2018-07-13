#ifndef PORTSCOMBOBOX_H
#define PORTSCOMBOBOX_H

#include <QComboBox>

class CPortEnumerator;

class CPortsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CPortsComboBox(QWidget* parent = 0);
    void showPopup();

    void setPortEnumerator(CPortEnumerator* pe);
    void setPort(QString port);

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private slots:
    void onEnumerationFinished();

private:
    CPortEnumerator* m_pe;
    QString m_portToBeSet;

    void refresh();
};

#endif // PORTSCOMBOBOX_H
