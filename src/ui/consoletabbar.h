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

#ifndef CONSOLETABBAR_H
#define CONSOLETABBAR_H

#include <QTabBar>

class QPushButton;
class MainWindow;

class ConsoleTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit ConsoleTabBar(QWidget* parent = nullptr);
    ~ConsoleTabBar();

    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void showEvent(QShowEvent* event);
    void tabInserted(int index);
    void tabRemoved(int index);

    void moveButton();
    void moveTab(int from, int to);

    MainWindow* getNewMainWindow() const
    {
        return newMainWindow_;
    }
    QPoint getClickOffset() const
    {
        return offset_;
    }

signals:
    void addButtonClicked();
    void tabDetached(int index);

public slots:

private:
    QPushButton* addTabButton_;
    MainWindow*  newMainWindow_;
    QPoint       offset_;
};

#endif // CONSOLETABBAR_H
