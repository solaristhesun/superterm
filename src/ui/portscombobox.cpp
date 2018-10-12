/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>

#include "ui/portscombobox.h"
#include "ui/portitemdelegate.h"
#include "serial/portenumerator.h"

PortsComboBox::PortsComboBox(QWidget* parent)
    : QComboBox(parent)
    , portEnumerator_(nullptr)
    , portToBeSet_("")
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
    portEnumerator_ = pe;
    connect(portEnumerator_, &PortEnumerator::enumerationFinished, this, &PortsComboBox::onEnumerationFinished);
    refresh();
}

void PortsComboBox::setPort(QString port)
{
    portToBeSet_ = port;
}

void PortsComboBox::showEvent(QShowEvent*)
{
    portEnumerator_->startEnumeration();
}

void PortsComboBox::hideEvent(QHideEvent*)
{
    portEnumerator_->stopEnumeration();
}

void PortsComboBox::onEnumerationFinished()
{
    if (!portToBeSet_.isEmpty())
    {
        refresh();
        this->setCurrentDeviceName(portToBeSet_);
        portToBeSet_ = "";
    }
}

void PortsComboBox::refresh()
{
    QString currentText = QComboBox::currentText();

    QComboBox::clear();
    QComboBox::addItem(tr("Select port"));

    for (SerialPortInfo portInfo : portEnumerator_->getAvailablePorts())
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
