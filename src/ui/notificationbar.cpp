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

#include <QDesktopServices>
#include <QUrl>

#include "notificationbar.h"
#include "ui_notificationbar.h"

NotificationBar::NotificationBar(QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::NotificationBar)
    , link_()
{
    ui_->setupUi(this);
}

NotificationBar::~NotificationBar()
{
    delete ui_;
}

void NotificationBar::setNotificationText(const QString& text)
{
    ui_->label->setText(text);
}

void NotificationBar::setLink(const QString& link)
{
    link_ = link;
}

void NotificationBar::openLinkInBrowser()
{
    QDesktopServices::openUrl(QUrl(link_));
}

// EOF <stefan@scheler.com>
