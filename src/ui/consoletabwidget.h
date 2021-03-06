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

#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class ConsoleTab;
class ConsoleTabBar;
class QMenu;

class ConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConsoleTabWidget(QWidget* parent = nullptr);
    ~ConsoleTabWidget() override;

    void        addTab(ConsoleTab* tab);
    void        setCurrentTabText(const QString& test);
    void        setConsoleFont(const QFont& font);
    void        aboutToQuit();
    void        destroyTab(const int index);
    ConsoleTab* currentWidget() const;
    ConsoleTab* widget(int index) const;

public slots:
    void closeTab(int index);
    void onAddButtonClicked();
    void onTabDetached(int index);
    void showContextMenu(const QPoint& pt);
    void showTabRenameDialog();
    void onTabCloseRequested(int index);
    void activateNextTab();
    void activatePrevTab();

private:
    void createContextMenu();

private:
    ConsoleTabBar* tabBar_;
    QMenu*         contextMenu_;
    QAction*       renameTabAction_;
};

#endif // CONSOLETABWIDGET_H
