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

#include "portitemview.h"
#include "ui_portitemview.h"

PortItemView::PortItemView(QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::PortItemView)
{
    ui_->setupUi(this);
}

PortItemView::~PortItemView()
{
    delete ui_;
}

void PortItemView::setDescription(const QString& description)
{
    ui_->descriptionLabel->setText(description);
}

void PortItemView::setPortName(const QString& portName)
{
    ui_->portNameLabel->setText(portName);
}

void PortItemView::setBusyFlag(const bool bBusy)
{
    if (bBusy)
    {
        ui_->busyLabel->setStyleSheet("background-color: red");
    }
    else
    {
        ui_->busyLabel->setStyleSheet("background-color: green");
    }
}

// EOF <stefan@scheler.com>
