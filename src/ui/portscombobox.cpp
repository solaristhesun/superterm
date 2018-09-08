#include <QDebug>

#include "ui/portscombobox.h"
#include "ui/portitemdelegate.h"
#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

CPortsComboBox::CPortsComboBox(QWidget* parent)
    : QComboBox(parent)
    , m_portToBeSet("")
{
    QComboBox::setItemDelegate(new PortItemDelegate());
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
        this->setCurrentDeviceName(m_portToBeSet);
        m_portToBeSet = "";
    }
}

void CPortsComboBox::refresh()
{
    QString currentText = this->currentText();

    QComboBox::clear();
    QComboBox::addItem(tr("Select port"));

    for (CSerialPortInfo portInfo : m_pe->getAvailablePorts())
    {
        QString title = QString("%1 [%2]").arg(portInfo.shortName(), portInfo.description());
        QComboBox::addItem(title, QVariant::fromValue<CSerialPortInfo>(portInfo));
    }

    this->setCurrentText(currentText);
}

CSerialPortInfo CPortsComboBox::currentPortInfo() const
{
    return qvariant_cast<CSerialPortInfo>(this->currentData());
}

void CPortsComboBox::setCurrentDeviceName(const QString& deviceName)
{
    for (auto idx = 0; idx < QComboBox::count(); idx++)
    {
        CSerialPortInfo info = qvariant_cast<CSerialPortInfo>(QComboBox::itemData(idx));
        if (info.portName() == deviceName)
        {
            QComboBox::setCurrentIndex(idx);
            break;
        }
    }
}

// EOF <stefan@scheler.com>
