#include <QSerialPortInfo>
#include <QDebug>

#include "portscombobox.h"
#include "portenumerator.h"

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
    qDebug() << "SHOW";
    m_pe->startEnumeration();
}

void CPortsComboBox::hideEvent(QHideEvent* event)
{
    qDebug() << "HIDE";
    m_pe->stopEnumeration();
}

void CPortsComboBox::refresh()
{
    clear();
    addItem(tr("Select port"));

    foreach (const QSerialPortInfo &info, m_pe->getAvailablePorts())
    {
        const QString title = QString("%1 [%2]").arg(info.portName(), info.description());
        addItem(title, QVariant(info.portName()));
    }

#if defined(Q_OS_LINUX)
    addItem("pts/5", QVariant("/dev/pts/5"));
#endif
}

// EOF <stefan@scheler.com>
