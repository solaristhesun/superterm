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

#include "renametabframe.h"
#include "ui_renametabframe.h"

RenameTabFrame::RenameTabFrame(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::RenameTabFrame)
{
    ui_->setupUi(this);
    QFrame::hide();
}

RenameTabFrame::~RenameTabFrame()
{
    delete ui_;
}

void RenameTabFrame::setTabLabel(const QString& text)
{
    ui_->labelEdit->setText(text);
}

QString RenameTabFrame::tabLabel() const
{
    return ui_->labelEdit->text();
}

void RenameTabFrame::show()
{
    QFrame::show();
    ui_->labelEdit->selectAll();
    ui_->labelEdit->setFocus();
}

void RenameTabFrame::onApplyClicked()
{
    emit applyPressed();
}

// EOF <stefan@scheler.com>
