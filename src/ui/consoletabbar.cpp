#include <QDebug>
#include <QMouseEvent>
#include <QShowEvent>
#include <QPushButton>
#include <QRect>
#include <QApplication>
#include "mainwindow.h"

#include "consoletab.h"
#include "consoletabbar.h"
#include "ui_mainwindow.h"

CConsoleTabBar::CConsoleTabBar(QWidget* parent)
    : QTabBar(parent)
    , m_btn(new QPushButton(this))
    , m_selectedIndex(-1)
    , m_prevIndex(-1)
    , mNewMainWindow(NULL)
{
    qDebug() << "CConsoleTabBar::CConsoleTabBar()";
    setFocusPolicy(Qt::NoFocus);
    setMovable(false);
    m_btn->setStyleSheet("QPushButton { border: 1px solid #cacaca; border-right: 0; border-top-left-radius: 2px; border-bottom-left-radius: 2px;margin: 1px;     background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #E1E1E1, stop: 0.4 #DDDDdd, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } QPushButton:hover, QPushButton:pressed { background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #fafafa, stop: 0.4 #f4f4f4, stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);} ");
    m_btn->setGeometry(QRect(0, 200, 27, 27 ));
    m_btn->setFocusPolicy(Qt::NoFocus);
    m_btn->setIcon(QIcon(":/icons/application_add_16x16.png"));

    connect(m_btn, &QPushButton::clicked, this, &CConsoleTabBar::addButtonClicked);
}

CConsoleTabBar::~CConsoleTabBar()
{
    qDebug() << "CConsoleTabBar::~CConsoleTabBar()";
}

void CConsoleTabBar::tabInserted(int)
{
    moveButton();
}

void CConsoleTabBar::tabRemoved(int)
{
    moveButton();
}

void CConsoleTabBar::moveButton()
{
    const int iLastTabIndex = count() - 1;

    if (iLastTabIndex != -1)
    {
        QRect rect = tabRect(iLastTabIndex);
        int y = rect.bottom();
        m_btn->move(rect.bottomRight().x()-28+3, y);
    }
}

void CConsoleTabBar::showEvent(QShowEvent *event)
{
    moveButton();
    QTabBar::showEvent(event);
}

void CConsoleTabBar::mouseReleaseEvent(QMouseEvent* event)
{
    QTabBar::mouseReleaseEvent(event);
    mNewMainWindow = NULL;
}

void CConsoleTabBar::setDetachCompleted()
{
   /*mNewMainWindow = nullptr;
   QMouseEvent event(QEvent::MouseButtonRelease, QPointF(),Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
   QTabBar::mouseReleaseEvent(&event);*/
}

void CConsoleTabBar::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    CMainWindow* currentWindow = static_cast<CMainWindow*>(QApplication::activeWindow());

    // dragging single tab moves window
    if (currentWindow->getTabCount() == 1)
    {
        currentWindow->move(event->globalPos() - mOffset);

        qDebug() << "MOVING" << currentWindow;

        int c = 0;
        foreach (QWidget *widget, QApplication::topLevelWidgets())
        {
            if (widget->objectName() == "CMainWindow" && widget != currentWindow)
            {
                qDebug() << c++ << widget;
                CMainWindow *w = static_cast<CMainWindow*>(widget);
                CConsoleTabBar *tabBar = static_cast<CConsoleTabBar*>(w->m_ui->tabWidget->tabBar());
                QRect globalWidgetsRect = QRect(tabBar->mapToGlobal(QPoint(0,0)), tabBar->size());

                if (globalWidgetsRect.contains(event->globalPos() - tabBar->getClickOffset()))
                {
                    qDebug() << "IN DROP ZONE" << globalWidgetsRect << event->globalPos();

                    // remove tab from current mainwindow
                    CConsoleTab *tab = currentWindow->detachTab();

                    qDebug() << "attaching tab" << tab->getLabel() << "from" << currentWindow << "to" << w;

                    // move tab to other mainwindow
                    w->addTab(tab);
                    w->activateWindow();

                    // destroy current main window
                    currentWindow->hide();
                    currentWindow->deleteLater();

                    break;
                }
            }
        }
        return;
    }
    else
    {
        if (!mNewMainWindow)
        {
            if (count() > 1)
            {
                if (pos.x() < - 25 || pos.x() > QTabBar::rect().width() + 25)
                {
                    mNewMainWindow = new CMainWindow;
                    mNewMainWindow->hide();
                    mNewMainWindow->setGeometry(QApplication::activeWindow()->geometry());
                    mNewMainWindow->show();
                    mNewMainWindow->move(event->globalPos() - mOffset);

                    emit tabDetached(currentIndex());
                }
                else
                {
                    QTabBar::mouseMoveEvent(event);
                }
            }
            else
            {
                QTabBar::mouseMoveEvent(event);
            }
        }
        else
        {
            qDebug() << "NOT MOVING";
            //mNewMainWindow->move(event->globalPos() - mOffset);
        }
    }
    //QTabBar::mouseMoveEvent(event);
}

void  CConsoleTabBar::mousePressEvent(QMouseEvent* event)
{
    QRect rect = tabRect(currentIndex());

    mOffset.setX(event->pos().x() - rect.x());
    mOffset.setY(event->pos().y() - rect.y());

    QMainWindow *mainWindow = static_cast<CMainWindow*>(QApplication::activeWindow());
    QSize decorationSize = mainWindow->frameSize()- mainWindow->size();

    mOffset.setX(mOffset.x() + decorationSize.width()/2);
    mOffset.setY(mOffset.y() + mainWindow->geometry().y()- mainWindow->frameGeometry().y());

    QTabBar::mousePressEvent(event);
}

// EOF <stefan@scheler.com>
