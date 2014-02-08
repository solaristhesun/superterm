#include <QSerialPortInfo>

#include "portscombobox.h"

CPortsComboBox::CPortsComboBox(QWidget *parent) :
    QComboBox(parent)
{
    refresh();
}

void CPortsComboBox::showPopup()
{
    refresh();
    QComboBox::showPopup();
}

void CPortsComboBox::refresh()
{
    clear();
    addItem(tr("Select port"));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        const QString title = QString("%1 [%2]").arg(info.portName(), info.description());
        addItem(title, QVariant(info.portName()));
    }

#if defined(Q_OS_LINUX)
    addItem("pts5", QVariant("/dev/pts/5"));
#endif
}

// EOF <stefan@scheler.com>
