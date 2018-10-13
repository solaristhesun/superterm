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

#include <QTabBar>
#include <QApplication>
#include <QDebug>
#include <QMenu>

#include "ui/consoletabwidget.h"
#include "ui/consoletabbar.h"
#include "ui/consoletab.h"
#include "ui/consoletabfactory.h"
#include "ui/mainwindow.h"
#include "serial/portenumerator.h"
#include "session/session.h"

ConsoleTabWidget::ConsoleTabWidget(QWidget* parent)
    : QTabWidget(parent)
    , tabBar_(new ConsoleTabBar(this))
    , contextMenu_(nullptr)
    , renameTabAction_(new QAction(tr("Rename tab..."), this))
{
    qDebug() << "ConsoleTabWidget::ConsoleTabWidget()";

    QTabWidget::setTabBar(tabBar_);

    createContextMenu();

    connect(tabBar_, &ConsoleTabBar::addButtonClicked, this, &ConsoleTabWidget::onAddButtonClicked);
    connect(tabBar_, &ConsoleTabBar::tabDetached, this, &ConsoleTabWidget::onTabDetached);
    connect(tabBar_, &ConsoleTabBar::customContextMenuRequested, this, &ConsoleTabWidget::showContextMenu);
    connect(renameTabAction_, &QAction::triggered, this, &ConsoleTabWidget::showTabRenameDialog);
    connect(this, &ConsoleTabWidget::tabCloseRequested, this, &ConsoleTabWidget::onTabCloseRequested);
}

ConsoleTabWidget::~ConsoleTabWidget()
{
    qDebug() << "ConsoleTabWidget::~ConsoleTabWidget()";
    delete tabBar_;
    delete renameTabAction_;
    delete contextMenu_;
}

void ConsoleTabWidget::showContextMenu(const QPoint& pt)
{
    if (tabBar_->tabAt(pt) == currentIndex())
    {
        contextMenu_->exec(mapToGlobal(pt));
    }
}

ConsoleTab* ConsoleTabWidget::currentWidget() const
{
    return static_cast<ConsoleTab*>(QTabWidget::currentWidget());
}

ConsoleTab* ConsoleTabWidget::widget(const int index) const
{
    return static_cast<ConsoleTab*>(QTabWidget::widget(index));
}

void ConsoleTabWidget::destroyTab(const int index)
{
    QWidget* tab = QTabWidget::widget(index);

    // remove tab from widget
    QTabWidget::removeTab(index);

    // delete tab
    if (tab)
    {
        delete tab;
    }
}

void ConsoleTabWidget::onTabCloseRequested(int index)
{
    ConsoleTab* tab = widget(index);
    tab->disconnectEndpoint();
}

void ConsoleTabWidget::closeTab(int index)
{
    destroyTab(index);

    // select next tab or close window
    if (QTabWidget::count() > 0)
    {
        QTabWidget::setCurrentIndex(index - 1);
    }
    else
    {
        QApplication::activeWindow()->close();
    }
}


void ConsoleTabWidget::activateNextTab()
{
    int currentIndex = QTabWidget::currentIndex();
    int maximumIndex = QTabWidget::count() - 1;

    if (currentIndex < maximumIndex)
    {
        currentIndex++;
    }
    else
    {
        currentIndex = 0;
    }

    QTabWidget::setCurrentIndex(currentIndex);
}

void ConsoleTabWidget::activatePrevTab()
{
    int currentIndex = QTabWidget::currentIndex();
    int maximumIndex = QTabWidget::count() - 1;

    if (currentIndex > 0)
    {
        currentIndex--;
    }
    else
    {
        currentIndex = maximumIndex;
    }

    QTabWidget::setCurrentIndex(currentIndex);
}

void ConsoleTabWidget::onTabDetached(int index)
{
    qDebug() << "[slot] onTabDetached";

    ConsoleTab* tab = widget(index);

    if (tab)
    {
        QTabWidget::removeTab(index);

        QObject::disconnect(tab, &ConsoleTab::labelChanged, this, &ConsoleTabWidget::setCurrentTabText);

        // add tab to new window
        MainWindow* newWin = tabBar_->getNewMainWindow();
        newWin->attachTab(tab);
    }
}

void ConsoleTabWidget::onAddButtonClicked()
{
    qDebug() << "[slot] onAddButtonClicked";

    addTab(ConsoleTabFactory::createTab());
}

void ConsoleTabWidget::addTab(ConsoleTab* tab)
{
    qDebug() << "adding tab" << tab->getLabel();

    QTabWidget::addTab(tab, tab->getLabel());
    QTabWidget::setCurrentWidget(tab);

    QObject::connect(tab, &ConsoleTab::labelChanged, this, &ConsoleTabWidget::setCurrentTabText);

    tabBar_->moveButton();
}

void ConsoleTabWidget::setConsoleFont(const QFont& font)
{
    // set font on all tabs
    for (int i = 0; i < QTabWidget::count(); i++)
    {
        ConsoleTab* tab = widget(i);
        tab->setConsoleFont(font);
    }
}

void ConsoleTabWidget::setCurrentTabText(const QString& text)
{
    const int curIndex = currentIndex();
    setTabText(curIndex, text);
    tabBar_->moveButton();
}


void ConsoleTabWidget::createContextMenu()
{
    renameTabAction_->setShortcut(Qt::Key_Control | Qt::Key_R);

    contextMenu_ = new QMenu(this);
    contextMenu_->addAction(renameTabAction_);
}

void ConsoleTabWidget::showTabRenameDialog()
{
    ConsoleTab* tab = currentWidget();
    tab->showRenameTabDialog();
}

// EOF <stefan@scheler.com>
