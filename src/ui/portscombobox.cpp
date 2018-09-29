#include <QDebug>

#include "ui/portscombobox.h"
#include "ui/portitemdelegate.h"
#include "serial/portenumerator.h"
#include "serial/serialportinfo.h"

PortsComboBox::PortsComboBox(QWidget* parent)
    : QComboBox(parent)
    , m_portToBeSet("")
{
    QComboBox::setItemDelegate(new PortItemDelegate());
}

void PortsComboBox::showPopup()
{
    refresh();
    QComboBox::showPopup();
}

void PortsComboBox::setPortEnumerator(PortEnumerator* pe)
{
    m_pe = pe;
    connect(m_pe, &PortEnumerator::enumerationFinished, this, &PortsComboBox::onEnumerationFinished);
    refresh();
}

void PortsComboBox::setPort(QString port)
{
    //this->setCurrentText(port);
    m_portToBeSet = port;
}

void PortsComboBox::showEvent(QShowEvent*)
{
    m_pe->startEnumeration();
}

void PortsComboBox::hideEvent(QHideEvent*)
{
    m_pe->stopEnumeration();
}

void PortsComboBox::onEnumerationFinished()
{
    if (!m_portToBeSet.isEmpty())
    {
        refresh();
        this->setCurrentDeviceName(m_portToBeSet);
        m_portToBeSet = "";
    }
}

void PortsComboBox::refresh()
{
    QString currentText = this->currentText();

    QComboBox::clear();
    QComboBox::addItem(tr("Select port"));

    for (SerialPortInfo portInfo : m_pe->getAvailablePorts())
    {
        QString title = QString("%1 [%2]").arg(portInfo.shortName(), portInfo.description());
        QComboBox::addItem(title, QVariant::fromValue<SerialPortInfo>(portInfo));
    }

    this->setCurrentText(currentText);
}

SerialPortInfo PortsComboBox::currentPortInfo() const
{
    return qvariant_cast<SerialPortInfo>(this->currentData());
}

void PortsComboBox::setCurrentDeviceName(const QString& deviceName)
{
    for (auto idx = 0; idx < QComboBox::count(); idx++)
    {
        SerialPortInfo info = qvariant_cast<SerialPortInfo>(QComboBox::itemData(idx));
        if (info.portName() == deviceName)
        {
            QComboBox::setCurrentIndex(idx);
            break;
        }
    }
}

// EOF <stefan@scheler.com>
