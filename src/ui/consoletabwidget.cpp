#include <iostream>
#include <QTabBar>
#include <QDebug>
#include <QApplication>

#include "consoletabwidget.h"
#include "consoletabbar.h"
#include "consoletab.h"

ConsoleTabWidget::ConsoleTabWidget(QWidget *parent) :
    QTabWidget(parent),
    m_tabBar(new ConsoleTabBar(this))
{
    setTabBar(m_tabBar);
    onAddButtonClicked();

    //QTabWidget::addTab(new ConsoleTab(this), "");

    connect(m_tabBar, SIGNAL(addButtonClicked()), this, SLOT(onAddButtonClicked()));
}

ConsoleTabWidget::~ConsoleTabWidget()
{
    delete m_tabBar;
}

void ConsoleTabWidget::onCloseTab(int index)
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

void ConsoleTabWidget::addTab(ConsoleTab *tab)
{
    int index = QTabWidget::addTab(tab, "New tab");
}

void ConsoleTabWidget::onAddButtonClicked(void)
{
    int index = QTabWidget::addTab(new ConsoleTab(this), "New tab");
    setCurrentIndex(index);
}

void ConsoleTabWidget::setCurrentTabText(const QString &text)
{
    const int curIndex = currentIndex();
    setTabText(curIndex, text);
    m_tabBar->moveButton();
}

// EOF <stefan@scheler.com>
