#include <QTabBar>
#include <QApplication>
#include <QDebug>

#include "consoletabwidget.h"
#include "consoletabbar.h"
#include "consoletab.h"
#include "ui/consoletabfactory.h"
#include "consoletab.h"
#include "mainwindow.h"
#include "enumerator/portenumerator.h"
#include "obj/session.h"

CConsoleTabWidget::CConsoleTabWidget(QWidget *parent)
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
        QTabWidget::setCurrentIndex(index-1);
    }
    else
    {
        QApplication::activeWindow()->close();
    }
}

void CConsoleTabWidget::onTabDetached(int index)
{
    qDebug() << "[slot] onTabDetached";

    if (QTabWidget::count() > 1)
    {
        CConsoleTab* tab = widget(index);

        QTabWidget::removeTab(index);

        QObject::disconnect(this, &CConsoleTabWidget::appQuits, tab, &CConsoleTab::onAppQuit);
        QObject::disconnect(tab, &CConsoleTab::labelChanged, this, &CConsoleTabWidget::setCurrentTabText);

        // add tab to new window
        CMainWindow *newWin = m_tabBar->getNewMainWindow();
        newWin->addTab(tab);
        m_tabBar->setDetachCompleted();
    }
}

void CConsoleTabWidget::onAddButtonClicked(void)
{
    qDebug() << "[slot] onAddButtonClicked";

    addTab(CConsoleTabFactory::createTab());
}

void CConsoleTabWidget::addTab(CConsoleTab* tab)
{
    QTabWidget::addTab(tab, tab->getLabel());
    QTabWidget::setCurrentWidget(tab);

    QObject::connect(this, &CConsoleTabWidget::appQuits, tab, &CConsoleTab::onAppQuit);
    QObject::connect(tab, &CConsoleTab::labelChanged, this, &CConsoleTabWidget::setCurrentTabText);

    m_tabBar->moveButton();
}

void CConsoleTabWidget::setConsoleFont(const QFont &font)
{
    // set font on all tabs
    for (int i = 0; i < QTabWidget::count(); i++)
    {
        CConsoleTab *tab = widget(i);
        tab->setConsoleFont(font);
    }
}

void CConsoleTabWidget::setCurrentTabText(const QString &text)
{
    const int curIndex = currentIndex();
    setTabText(curIndex, text);
    m_tabBar->moveButton();
}

void CConsoleTabWidget::aboutToQuit(void)
{
    emit appQuits();
}

// EOF <stefan@scheler.com>
