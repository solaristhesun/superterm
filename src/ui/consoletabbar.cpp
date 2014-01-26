#include <QDebug>
#include <QMouseEvent>
#include <QShowEvent>

#include "consoletabbar.h"

ConsoleTabBar::ConsoleTabBar(QWidget *parent) :
    QTabBar(parent),
    m_selectedIndex(-1),
    m_prevIndex(-1)
{
    setFocusPolicy(Qt::NoFocus);

    // add empty tab
    addTab("");
}

void ConsoleTabBar::showEvent(QShowEvent *event)
{
    qDebug() << "show";
    tabButton(lastTabIndex(), QTabBar::RightSide)->resize(0, 0);

    QTabBar::showEvent(event);
}

void ConsoleTabBar::mouseReleaseEvent(QMouseEvent * event)
{
     const QPoint pos = event->pos();

     if (tabAt(pos) == lastTabIndex())
     {
         emit addButtonClicked();
         return;
     }
     if (tabAt(pos) == -1)
     {
         setCurrentIndex(m_prevIndex);
         qDebug() << "FOO";
     }

     m_selectedIndex = -1;

     QTabBar::mouseReleaseEvent(event);
}

void ConsoleTabBar::mouseMoveEvent(QMouseEvent* event)
{
#if 1
    const QPoint pos = event->pos();
    qDebug() << tabButton(m_selectedIndex,QTabBar::RightSide)->pos();

    if (m_selectedIndex == lastTabIndex())
    {
        event->ignore();
        return;
    }
#endif
    QTabBar::mouseMoveEvent(event);
}

void  ConsoleTabBar::mousePressEvent(QMouseEvent * event)
{
    m_prevIndex = currentIndex();
    m_selectedIndex = tabAt(event->pos());
    QTabBar::mousePressEvent(event);
}

int ConsoleTabBar::lastTabIndex(void)
{
    return count()-1;
}

// EOF <stefan@scheler.com>
