#include <QDebug>
#include <QMouseEvent>
#include <QShowEvent>

#include "consoletabbar.h"

ConsoleTabBar::ConsoleTabBar(QWidget *parent) :
    QTabBar(parent),
    m_currentIndex(0)
{
    setFocusPolicy(Qt::NoFocus);

    // add tab without close button
    addTab("");
}

void ConsoleTabBar::showEvent(QShowEvent *event)
{
    tabButton(lastIndex(), QTabBar::RightSide)->resize(0, 0);
    QTabBar::showEvent(event);
}

void ConsoleTabBar::mouseReleaseEvent(QMouseEvent * event)
{
     const QPoint pos = event->pos();

     if (this->tabAt(pos) == lastIndex())
     {
         emit addButtonClicked();
         return;
     }

     QTabBar::mouseReleaseEvent(event);
}

int ConsoleTabBar::lastIndex(void)
{
    return count()-1;
}
