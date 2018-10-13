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

#ifndef STATUSBARFRAME_H
#define STATUSBARFRAME_H

#include <QFrame>

namespace Ui
{
class StatusBarFrame;
}

class StatusBarFrame : public QFrame
{
    Q_OBJECT

public:
    explicit StatusBarFrame(QWidget* parent = nullptr);
    ~StatusBarFrame() override;

    void showMessage(const QString& message, const int timeout = 0);
    void showProgressMessage(const QString& message);
    void hideProgressMessage();

public slots:
    void hideOnTimeout();

signals:
    void cancelReconnection();

private:
    Ui::StatusBarFrame* ui_;

    void setStyleProperty(const char* name, const QVariant& variant);
    void setupWaitingSpinner();
};

#endif // STATUSBARFRAME_H
