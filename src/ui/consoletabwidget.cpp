#include <iostream>
#include <QTabBar>
#include <QDebug>
#include <QApplication>

#include "consoletabwidget.h"
#include "consoletabbar.h"
#include "consoletab.h"

CConsoleTabWidget::CConsoleTabWidget(QWidget *parent) :
    QTabWidget(parent),
    m_tabBar(new CConsoleTabBar(this))
{
    setTabBar(m_tabBar);
    addTab();

    connect(m_tabBar, SIGNAL(addButtonClicked()), this, SLOT(addTab()));
}

CConsoleTabWidget::~CConsoleTabWidget()
{
    delete m_tabBar;
}

void CConsoleTabWidget::closeTab(int index)
{
    std::cout << "CLOSE" <<std::endl;
#if 0
    QWidget *wdgt = ui->tabWidget->widget( nCurIndex );
       ui->tabWidget->removeTab(  nCurIndex );
       if( wdgt )
           delete wdgt;
#endif
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

void CConsoleTabWidget::addTab(void)
{
    int index = QTabWidget::addTab(new CConsoleTab(this), tr("New tab"));
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
