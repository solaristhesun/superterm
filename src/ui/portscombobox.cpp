#include <QDebug>

#include "portscombobox.h"
#include "enumerator/portenumerator.h"
#include "enumerator/serialportinfo.h"

CPortsComboBox::CPortsComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void CPortsComboBox::showPopup()
{
    refresh();
    QComboBox::showPopup();
}

void CPortsComboBox::setPortEnumerator(CPortEnumerator *pe)
{
    m_pe = pe;
    refresh();
}

void CPortsComboBox::showEvent(QShowEvent* event)
{
    m_pe->startEnumeration();
}

void CPortsComboBox::hideEvent(QHideEvent* event)
{
    m_pe->stopEnumeration();
}

void CPortsComboBox::refresh()
{
    clear();
    addItem(tr("Select port"));

    foreach (const CSerialPortInfo &portInfo, m_pe->getAvailablePorts())
    {
        const QString title = QString("%1 [%2]").arg(portInfo.getShortName(), portInfo.getDescription());
        qDebug () << "ADD " << title;
        addItem(title, QVariant(portInfo.getPortName()));
    }
}

// EOF <stefan@scheler.com>
