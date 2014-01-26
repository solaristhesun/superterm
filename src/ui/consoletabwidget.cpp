#include <iostream>
#include <QTabBar>

#include "consoletabwidget.h"
#include "consoletab.h"

ConsoleTabWidget::ConsoleTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    tabBar()->setFocusPolicy(Qt::NoFocus);
}

void ConsoleTabWidget::closeTab(int index)
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
}

void ConsoleTabWidget::addTab(ConsoleTab *tab)
{
    int index = QTabWidget::addTab(tab, "");
    m_origButtonSize = tabBar()->tabButton(index, QTabBar::RightSide)->size();
    tabBar()->tabButton(index, QTabBar::RightSide)->resize(0, 0);
}

void ConsoleTabWidget::setCurrentTabText(const QString &text)
{
    const int curIndex = currentIndex();
    tabBar()->tabButton(curIndex, QTabBar::RightSide)->resize(m_origButtonSize);
    setTabText(curIndex, text);
}

// EOF <stefan@scheler.com>
