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

#include <QMoveEvent>
#include <QDebug>
#include <QRect>
#include <QString>
#include <QShortcut>

#if defined(Q_OS_WIN)
#include "windows.h"
#include "math.h"
#endif

#include "ui_mainwindow.h"
#include "ui/mainwindow.h"
#include "ui/consoletab.h"
#include "ui/consoletabbar.h"
#include "ui/consoletabfactory.h"
#include "misc/globals.h"
#include "misc/softwareversion.h"
#include "session/session.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , shortcutNextTab_(new QShortcut(QKeySequence("Alt+Right"), this))
    , shortcutPrevTab_(new QShortcut(QKeySequence("Alt+Left"), this))
{
    qDebug() << "MainWindow::MainWindow()";

    ui_->setupUi(this);
    ui_->notificationBar->hide();
    QWidget::resize(800, 600);

    QMainWindow::setWindowTitle(Globals::ApplicationFullName);

    connect(shortcutNextTab_, &QShortcut::activated, ui_->tabWidget, &ConsoleTabWidget::activateNextTab);
    connect(shortcutPrevTab_, &QShortcut::activated, ui_->tabWidget, &ConsoleTabWidget::activatePrevTab);
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow::~MainWindow()" << this;
    delete shortcutNextTab_;
    delete shortcutPrevTab_;
    delete ui_;
}

QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void MainWindow::addExistingTabsFromFile()
{
    QStringList fileNames = Session::getSessionList();

    if (!fileNames.isEmpty())
    {
        for (const QString& fileName : fileNames)
        {
            Session* session = Session::createSessionFromFile(fileName);

            ui_->tabWidget->addTab(ConsoleTabFactory::createTabFromSession(session));
        }
    }
    else
    {
        ui_->tabWidget->addTab(ConsoleTabFactory::createTab());
    }
}

void MainWindow::attachTab(ConsoleTab* tab)
{
    qDebug() << "attaching tab" << tab->getLabel() << "to" << this;

    ui_->tabWidget->addTab(tab);
}


ConsoleTab* MainWindow::detachTab()
{
    int curIndex = ui_->tabWidget->currentIndex();

    ConsoleTab* tab = ui_->tabWidget->widget(curIndex);
    ui_->tabWidget->removeTab(curIndex);

    qDebug() << "detaching tab" << tab->getLabel() << "from" << this;

    return tab;
}

void MainWindow::showUpdateInfo(const SoftwareVersion& version)
{
    ui_->notificationBar->setNotificationText(QString(tr("A software update is available. Click here to download superterm %1.")).arg(version.toString()));
    ui_->notificationBar->setLink(Globals::ApplicationWebsite);
    ui_->notificationBar->show();
}

void MainWindow::onSecondaryInstanceLaunched()
{
    qDebug() << "[slot] onSecondaryInstanceLaunched";

    MainWindow* mainWindow = new MainWindow;

    mainWindow->show();
    mainWindow->attachTab(ConsoleTabFactory::createTab());
}

bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long*)
{
    bool bEventHandled = false;

#if defined(Q_OS_WIN)
    if (eventType == "windows_generic_MSG")
    {
        MSG* msg = static_cast< MSG* >( message );

        switch ( msg->message )
        {
            case WM_SIZING:
                {
                    RECT* r = reinterpret_cast<RECT*>(msg->lParam);

                    long widthWindow = r->right - r->left;
                    long heightWindow = r->bottom - r->top;

                    qDebug() << widthWindow << heightWindow;
                    ConsoleTab* tab = ui_->tabWidget->widget(0); // FIXME: ugly

                    QSize decorationSize = QWidget::frameSize() - QWidget::size();
                    QSize charSize = tab->getCharWidth();

                    int tabBarWidth = QWidget::width() - tab->getViewPortSize().width();

                    // calculate adjusted view dimensions
                    int adjustedViewWidth = floor((widthWindow - decorationSize.width() - tabBarWidth) / charSize.width()) * charSize.width() + 3;
                    int adjustedViewHeight = floor((heightWindow - decorationSize.height()) / charSize.height()) * charSize.height() + 3;

                    // manipulate event
                    r->right = r->left + decorationSize.width() + tabBarWidth + adjustedViewWidth;
                    r->bottom = r->top + decorationSize.height() + adjustedViewHeight;
                }
                break;
            default:
                break;
        }
    }
#elif defined(Q_OS_LINUX)
    if (eventType == "xcb_generic_event_t")
    {
        Q_UNUSED(message);
    }
#else
    Q_UNUSED(eventType)
    Q_UNUSED(message);
#endif

    return bEventHandled;
}

int MainWindow::getTabCount() const
{
    return ui_->tabWidget->count();
}

QRect MainWindow::getTabBarRect() const
{
    ConsoleTabBar* tabBar = static_cast<ConsoleTabBar*>(ui_->tabWidget->tabBar());
    return QRect(tabBar->mapToGlobal(QPoint(0, 0)), tabBar->size());
}

// EOF <stefan@scheler.com>
