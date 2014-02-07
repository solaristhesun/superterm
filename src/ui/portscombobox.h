#ifndef PORTSCOMBOBOX_H
#define PORTSCOMBOBOX_H

#include <QComboBox>

class CPortsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CPortsComboBox(QWidget *parent = 0);
    void showPopup(void);

signals:

public slots:

private:
    void refresh(void);
};

#endif // PORTSCOMBOBOX_H
