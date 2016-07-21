#include <QTabBar>
#include <QApplication>
#include <QDebug>

#include "ui/consoletabwidget.h"
#include "ui/consoletabbar.h"
#include "ui/consoletab.h"
#include "ui/consoletabfactory.h"
#include "ui/consoletab.h"
#include "ui/mainwindow.h"
#include "serial/portenumerator.h"
#include "session/session.h"

CConsoleTabWidget::CConsoleTabWidget(QWidget* parent)
    : QTabWidget(parent)
    , m_tabBar(new CConsoleTabBar(this))
{
    qDebug() << "CConsoleTabWidget::CConsoleTabWidget()";

    QTabWidget::setTabBar(m_tabBar);

    connect(m_tabBar, &CConsoleTabBar::addButtonClicked, this, &CConsoleTabWidget::onAddButtonClicked);
    connect(m_tabBar, &CConsoleTabBar::tabDetached, this, &CConsoleTabWidget::onTabDetached);
}

CConsoleTabWidget::~CConsoleTabWidget()
{
    qDebug() << "CConsoleTabWidget::~CConsoleTabWidget()";
    delete m_tabBar;
}

CConsoleTab* CConsoleTabWidget::currentWidget() const
{
    return static_cast<CConsoleTab*>(QTabWidget::currentWidget());
}

CConsoleTab* CConsoleTabWidget::widget(int index) const
{
    return static_cast<CConsoleTab*>(QTabWidget::widget(index));
}

void CConsoleTabWidget::destroyTab(int index)
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

void CConsoleTabWidget::closeTab(int index)
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

void CConsoleTabWidget::onTabDetached(int index)
{
    qDebug() << "[slot] onTabDetached";

    CConsoleTab* tab = widget(index);

    if (tab)
    {
        QTabWidget::removeTab(index);

        QObject::disconnect(tab, &CConsoleTab::labelChanged, this, &CConsoleTabWidget::setCurrentTabText);

        // add tab to new window
        CMainWindow* newWin = m_tabBar->getNewMainWindow();
        newWin->attachTab(tab);
    }
}

void CConsoleTabWidget::onAddButtonClicked()
{
    qDebug() << "[slot] onAddButtonClicked";

    addTab(CConsoleTabFactory::createTab());
}

void CConsoleTabWidget::addTab(CConsoleTab* tab)
{
    qDebug() << "adding tab" << tab->getLabel();

    QTabWidget::addTab(tab, tab->getLabel());
    QTabWidget::setCurrentWidget(tab);

    QObject::connect(tab, &CConsoleTab::labelChanged, this, &CConsoleTabWidget::setCurrentTabText);

    m_tabBar->moveButton();
}

void CConsoleTabWidget::setConsoleFont(const QFont& font)
{
    // set font on all tabs
    for (int i = 0; i < QTabWidget::count(); i++)
    {
        CConsoleTab* tab = widget(i);
        tab->setConsoleFont(font);
    }
}

void CConsoleTabWidget::setCurrentTabText(const QString& text)
{
    const int curIndex = currentIndex();
    setTabText(curIndex, text);
    m_tabBar->moveButton();
}

// EOF <stefan@scheler.com>
