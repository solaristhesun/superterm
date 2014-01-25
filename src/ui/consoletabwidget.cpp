#include <iostream>


#include "consoletabwidget.h"

ConsoleTabWidget::ConsoleTabWidget(QWidget *parent) :
    QTabWidget(parent)
{

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
