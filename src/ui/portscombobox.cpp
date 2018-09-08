#include <QDebug>

#include "ui/portscombobox.h"
#include "ui/portitemdelegate.h"
#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

CPortsComboBox::CPortsComboBox(QWidget* parent)
    : QComboBox(parent)
    , m_portToBeSet("")
{
    // currently nothing
    //QComboBox::setItemDelegate(new PortItemDelegate());
}

void CPortsComboBox::showPopup()
{
    refresh();
    QComboBox::showPopup();
}

void CPortsComboBox::setPortEnumerator(CPortEnumerator* pe)
{
    m_pe = pe;
    connect(m_pe, &CPortEnumerator::enumerationFinished, this, &CPortsComboBox::onEnumerationFinished);
    refresh();
}

void CPortsComboBox::setPort(QString port)
{
    //this->setCurrentText(port);
    m_portToBeSet = port;
}

void CPortsComboBox::showEvent(QShowEvent*)
{
    m_pe->startEnumeration();
}

void CPortsComboBox::hideEvent(QHideEvent*)
{
    m_pe->stopEnumeration();
}

void CPortsComboBox::onEnumerationFinished()
{
    if (!m_portToBeSet.isEmpty())
    {
        refresh();
        this->setCurrentText(m_portToBeSet);
        m_portToBeSet = "";
    }
}

void CPortsComboBox::refresh()
{
    QString currentText = this->currentText();

    clear();
    addItem(tr("Select port"));

    for (const CSerialPortInfo& portInfo : m_pe->getAvailablePorts())
    {
        QString title = QString("%1 [%2]").arg(portInfo.shortName(), portInfo.getDescription());

        if (portInfo.isBusy())
        {
            title.prepend("[" + tr("busy") + "] ");
        }

        addItem(title, QVariant(portInfo.portName()));
    }

    this->setCurrentText(currentText);
}

// EOF <stefan@scheler.com>
