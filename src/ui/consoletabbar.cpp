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
#include <QMouseEvent>
#include <QShowEvent>
#include <QPushButton>
#include <QRect>
#include <QApplication>
#include <QStyle>

#include "mainwindow.h"

#include "ui/consoletab.h"
#include "ui/consoletabbar.h"

ConsoleTabBar::ConsoleTabBar(QWidget* parent)
    : QTabBar(parent)
    , addTabButton_(new QPushButton(this))
    , newMainWindow_(nullptr)
{
    qDebug() << "CConsoleTabBar::CConsoleTabBar()";

    QTabBar::setFocusPolicy(Qt::NoFocus);
    QTabBar::setMovable(false);
    QTabBar::setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    addTabButton_->setObjectName(QStringLiteral("addTabBtn"));
    addTabButton_->setGeometry(QRect(0, 200, 26, 26));
    addTabButton_->setFocusPolicy(Qt::NoFocus);
    addTabButton_->setIcon(QIcon(":/icons/application_add_16x16.png"));

    connect(addTabButton_, &QPushButton::clicked, this, &ConsoleTabBar::addButtonClicked);
}

ConsoleTabBar::~ConsoleTabBar()
{
    qDebug() << "CConsoleTabBar::~CConsoleTabBar()";
}

void ConsoleTabBar::tabInserted(int)
{
    moveButton();
}

void ConsoleTabBar::tabRemoved(int)
{
    moveButton();
}

void ConsoleTabBar::moveButton()
{
    const int iLastTabIndex = count() - 1;

    if (iLastTabIndex != -1)
    {
        QRect rect = tabRect(iLastTabIndex);
        addTabButton_->move(rect.bottomRight().x() - 28+1, rect.bottom()+4);
    }
}

void ConsoleTabBar::showEvent(QShowEvent* event)
{
    moveButton();
    QTabBar::showEvent(event);
}

void ConsoleTabBar::mouseReleaseEvent(QMouseEvent* event)
{
    QTabBar::mouseReleaseEvent(event);
    newMainWindow_ = nullptr;
}

void ConsoleTabBar::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    QPoint globalPos = event->globalPos() - offset_;

    MainWindow* currentWindow = static_cast<MainWindow*>(QApplication::activeWindow());

    if (!currentWindow)
    {
        return;
    }

    // dragging single tab moves window
    if (currentWindow->getTabCount() == 1)
    {
        currentWindow->move(globalPos);

        // check if tab is over tabbar of other mainwindow
        for (QWidget* widget : QApplication::topLevelWidgets())
        {
            if (widget->inherits("QMainWindow") && widget != currentWindow)
            {
                MainWindow* otherWindow = static_cast<MainWindow*>(widget);
                QRect        tabBarRect = otherWindow->getTabBarRect();

                // check if mouse is in tabbar rect
                if (tabBarRect.contains(globalPos))
                {
                    // move tab to other mainwindow
                    otherWindow->attachTab(currentWindow->detachTab());
                    otherWindow->activateWindow();

                    // destroy current main window
                    currentWindow->hide();
                    currentWindow->deleteLater();

                    // FIXME: maybe emit(tabAttached) and move code to CTabWidget

                    break; // for
                }
            }
        }
    }
    else
    {
        if (!newMainWindow_)
        {
            const int iThreshold = 25;

            if (pos.x() < -iThreshold || pos.x() > QTabBar::rect().width() + iThreshold)
            {
                newMainWindow_ = new MainWindow;
                newMainWindow_->hide();
                newMainWindow_->setGeometry(QApplication::activeWindow()->geometry());
                newMainWindow_->show();
                newMainWindow_->move(globalPos);

                emit tabDetached(currentIndex());
            }
            else
            {
                QTabBar::mouseMoveEvent(event);
            }
        }
    }
}

void ConsoleTabBar::mousePressEvent(QMouseEvent* event)
{
    QRect rect = tabRect(currentIndex());

    offset_.setX(event->pos().x() - rect.x());
    offset_.setY(event->pos().y() - rect.y());

    QMainWindow* mainWindow = static_cast<MainWindow*>(QApplication::activeWindow());
    QSize        decorationSize = mainWindow->frameSize() - mainWindow->size();

    offset_.setX(offset_.x() + decorationSize.width() / 2);
    offset_.setY(offset_.y() + mainWindow->geometry().y() - mainWindow->frameGeometry().y());

    QTabBar::mousePressEvent(event);
}

// EOF <stefan@scheler.com>
