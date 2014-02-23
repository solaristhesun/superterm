#include <QTabBar>
#include <QApplication>

#include "consoletabwidget.h"
#include "consoletabbar.h"
#include "consoletab.h"
#include "enumerator/portenumerator.h"

CConsoleTabWidget::CConsoleTabWidget(QWidget *parent) :
    QTabWidget(parent),
    m_tabBar(new CConsoleTabBar(this)),
    m_pe(new CPortEnumerator())
{
    setTabBar(m_tabBar);
    addNewTab();

    connect(m_tabBar, SIGNAL(addButtonClicked()), this, SLOT(addNewTab()));
}

CConsoleTabWidget::~CConsoleTabWidget()
{
    delete m_tabBar;
}

void CConsoleTabWidget::closeTab(int index)
{
    QWidget *wdgt = widget( index );
    removeTab(  index );
    if( wdgt )
      delete wdgt;
    if (count() > 0)
    {
        setCurrentIndex(index-1);
    }
    else
    {
        QApplication::quit();
    }
}

void CConsoleTabWidget::addNewTab(void)
{
    int index = QTabWidget::addTab(new CConsoleTab(m_pe, this), tr("New tab"));
    setCurrentIndex(index);
    m_tabBar->moveButton();
}

void CConsoleTabWidget::setConsoleFont(const QFont &font)
{
    // set font on all tabs
    for (int i = 0; i < count(); i++)
    {
        CConsoleTab *tab = static_cast<CConsoleTab*>(widget(i));
        tab->setConsoleFont(font);
    }
}

void CConsoleTabWidget::setCurrentTabText(const QString &text)
{
    const int curIndex = currentIndex();
    setTabText(curIndex, text);
    m_tabBar->moveButton();
}

// EOF <stefan@scheler.com>
