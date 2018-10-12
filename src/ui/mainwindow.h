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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConsoleTab;
class SoftwareVersion;
class QShortcut;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QSize       sizeHint() const;
    void        attachTab(ConsoleTab* tab);
    void        addExistingTabsFromFile();
    ConsoleTab* detachTab();
    int         getTabCount() const;
    QRect       getTabBarRect() const;

private:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

public slots:
    void onSecondaryInstanceLaunched();
    void showUpdateInfo(const SoftwareVersion& version);

signals:
    void willQuit();

private:
    Ui::MainWindow* ui_;
    QShortcut*      shortcutNextTab_;
    QShortcut*      shortcutPrevTab_;
};

#endif // MAINWINDOW_H
