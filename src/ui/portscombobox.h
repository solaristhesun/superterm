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

#ifndef PORTSCOMBOBOX_H
#define PORTSCOMBOBOX_H

#include <QComboBox>

#include "serial/serialportinfo.h"

class PortEnumerator;

class PortsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit PortsComboBox(QWidget* parent = nullptr);
    void showPopup() override;

    void setPortEnumerator(PortEnumerator* pe);
    void setPort(QString port);

    SerialPortInfo currentPortInfo() const;
    void           setCurrentDeviceName(const QString& deviceName);

    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private slots:
    void onEnumerationFinished();

private:
    PortEnumerator* portEnumerator_;
    QString         portToBeSet_;

    void refresh();
};

#endif // PORTSCOMBOBOX_H
