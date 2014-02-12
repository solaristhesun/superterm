#ifndef PORTSCOMBOBOX_H
#define PORTSCOMBOBOX_H

#include <QComboBox>

class CPortEnumerator;

class CPortsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CPortsComboBox(QWidget *parent = 0);
    void showPopup(void);

    void setPortEnumerator(CPortEnumerator *pe);


    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

signals:

public slots:

private:
    void refresh(void);

    CPortEnumerator* m_pe;
};

#endif // PORTSCOMBOBOX_H
