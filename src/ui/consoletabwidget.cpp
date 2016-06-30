#include <QTabBar>
#include <QApplication>
#include <QDebug>

#include "consoletabwidget.h"
#include "consoletabbar.h"
#include "consoletab.h"
#include "mainwindow.h"
#include "enumerator/portenumerator.h"
#include "obj/session.h"

CConsoleTabWidget::CConsoleTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_tabBar(new CConsoleTabBar(this))
    , m_pe(new CPortEnumerator())
{
    qDebug() << "CConsoleTabWidget::CConsoleTabWidget()";

    setTabBar(m_tabBar);
    addNewTab(NULL);

    connect(m_tabBar, &CConsoleTabBar::addButtonClicked, this, &CConsoleTabWidget::onAddButtonClicked);
    connect(m_tabBar, &CConsoleTabBar::tabDetached, this, &CConsoleTabWidget::onTabDetached);
}

CConsoleTabWidget::~CConsoleTabWidget()
{
    qDebug() << "CConsoleTabWidget::~CConsoleTabWidget()";
    delete m_tabBar;
    delete m_pe;
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

    // select next tab or quit
    if (QTabWidget::count() > 0)
    {
        QTabWidget::setCurrentIndex(index-1);
    }
    else
    {
        QApplication::quit();
    }
}

void CConsoleTabWidget::onTabDetached(int index)
{
    qDebug() << "[slot] onTabDetached";

    if (count() > 1)
    {
        QWidget* tab = QTabWidget::widget(index);
        QString tabText = QTabWidget::tabText(index);

        QTabWidget::removeTab(index);

        CMainWindow *newWin = m_tabBar->getNewMainWindow();
        newWin->addTab(tab, tabText);
    }
}

void CConsoleTabWidget::onAddButtonClicked(void)
{
    qDebug() << "[slot] onAddButtonClicked";

    addNewTab(NULL);
}

void CConsoleTabWidget::addNewTab(CSession* session)
{
    QString tabText = session ? session->getDeviceName() : tr("New tab");
    m_pe->startEnumeration();
    CConsoleTab* tab = new CConsoleTab(m_pe, this, session);
    qDebug() << tabText;
    int index = QTabWidget::addTab(tab, tabText);
    setCurrentIndex(index);

    QObject::connect(this, SIGNAL(appQuits()), tab, SLOT(onAppQuit()));

    m_tabBar->moveButton();
}

void CConsoleTabWidget::setConsoleFont(const QFont &font)
{
    // set font on all tabs
    for (int i = 0; i < QTabWidget::count(); i++)
    {
        CConsoleTab *tab = static_cast<CConsoleTab*>(QTabWidget::widget(i));
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
