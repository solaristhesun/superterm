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

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private:

    CPortEnumerator* m_pe;

    void refresh();
};

#endif // PORTSCOMBOBOX_H
