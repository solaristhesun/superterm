#include <QDebug>
#include <QMouseEvent>
#include <QShowEvent>
#include <QPushButton>
#include <QRect>
#include <QApplication>
#include "mainwindow.h"

#include "ui/consoletab.h"
#include "ui/consoletabbar.h"

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
        int   y = rect.bottom();
        m_btn->move(rect.bottomRight().x() - 28 + 3, y);
    }
}

void CConsoleTabBar::showEvent(QShowEvent* event)
{
    moveButton();
    QTabBar::showEvent(event);
}

void CConsoleTabBar::mouseReleaseEvent(QMouseEvent* event)
{
    QTabBar::mouseReleaseEvent(event);
    mNewMainWindow = NULL;
}

void CConsoleTabBar::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    QPoint globalPos = event->globalPos() - mOffset;

    CMainWindow* currentWindow = static_cast<CMainWindow*>(QApplication::activeWindow());

    if (!currentWindow)
    {
        return;
    }

    // dragging single tab moves window
    if (currentWindow->getTabCount() == 1)
    {
        currentWindow->move(globalPos);

        // check if tab is over tabbar of other mainwindow
        for (QWidget* widget: QApplication::topLevelWidgets())
        {
            if (widget->objectName() == "CMainWindow" && widget != currentWindow)
            {
                CMainWindow* otherWindow = static_cast<CMainWindow*>(widget);
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
        if (!mNewMainWindow)
        {
            const int iThreshold = 25;

            if (pos.x() < -iThreshold || pos.x() > QTabBar::rect().width() + iThreshold)
            {
                mNewMainWindow = new CMainWindow;
                mNewMainWindow->hide();
                mNewMainWindow->setGeometry(QApplication::activeWindow()->geometry());
                mNewMainWindow->show();
                mNewMainWindow->move(globalPos);

                emit tabDetached(currentIndex());
            }
            else
            {
                QTabBar::mouseMoveEvent(event);
            }
        }
    }
}

void CConsoleTabBar::mousePressEvent(QMouseEvent* event)
{
    QRect rect = tabRect(currentIndex());

    mOffset.setX(event->pos().x() - rect.x());
    mOffset.setY(event->pos().y() - rect.y());

    QMainWindow* mainWindow = static_cast<CMainWindow*>(QApplication::activeWindow());
    QSize        decorationSize = mainWindow->frameSize() - mainWindow->size();

    mOffset.setX(mOffset.x() + decorationSize.width() / 2);
    mOffset.setY(mOffset.y() + mainWindow->geometry().y() - mainWindow->frameGeometry().y());

    QTabBar::mousePressEvent(event);
}

// EOF <stefan@scheler.com>
