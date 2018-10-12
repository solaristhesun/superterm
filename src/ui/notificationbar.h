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

#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include <QFrame>

namespace Ui
{
class NotificationBar;
}

class NotificationBar : public QFrame
{
    Q_OBJECT

public:
    explicit NotificationBar(QWidget* parent = nullptr);
    ~NotificationBar();

    void setNotificationText(const QString& text);
    void setLink(const QString& link);

public slots:
    void openLinkInBrowser();

private:
    Ui::NotificationBar* ui_;
    QString              link_;
};

#endif // NOTIFICATIONBAR_H
