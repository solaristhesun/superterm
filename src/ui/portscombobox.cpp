#include <QDebug>

#include "ui/portscombobox.h"
#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

CPortsComboBox::CPortsComboBox(QWidget* parent)
    : QComboBox(parent)
{
    // currently nothing
}

void CPortsComboBox::showPopup()
{
    refresh();
    QComboBox::showPopup();
}

void CPortsComboBox::setPortEnumerator(CPortEnumerator* pe)
{
    m_pe = pe;
    refresh();
}

void CPortsComboBox::showEvent(QShowEvent*)
{
    m_pe->startEnumeration();
}

void CPortsComboBox::hideEvent(QHideEvent*)
{
    m_pe->stopEnumeration();
}

void CPortsComboBox::refresh()
{
    clear();
    addItem(tr("Select port"));

    for (const CSerialPortInfo& portInfo : m_pe->getAvailablePorts())
    {
        const QString title = QString("%1 [%2]").arg(portInfo.getShortName(), portInfo.getDescription());
        qDebug () << "ADD " << title;
        addItem(title, QVariant(portInfo.getPortName()));
    }
}

// EOF <stefan@scheler.com>
