#include <QDebug>
#include <QMouseEvent>
#include <QShowEvent>
#include <QPushButton>

#include "consoletabbar.h"

ConsoleTabBar::ConsoleTabBar(QWidget *parent) :
    QTabBar(parent),
    m_btn(new QPushButton(this)),
    m_selectedIndex(-1),
    m_prevIndex(-1)
{
    setFocusPolicy(Qt::NoFocus);
    setMovable(false);
   // m_btn->setStyleSheet("QPushButton {border: 1px solid white; background; qlineargradient(); border-radius: 2px; } QPushButton:pressed { background-color: yellow; } QPushButton:hover { background-color: blue; }");
    m_btn->setGeometry(QRect(0, 200, 28, 28 ));
    m_btn->setFocusPolicy(Qt::NoFocus);
    connect(m_btn, SIGNAL(clicked()), this, SIGNAL(addButtonClicked()));
    // add empty tab
    //addTab("");
    moveButton();
}


void	ConsoleTabBar::tabInserted(int index)
{
    moveButton();
}

void	ConsoleTabBar::tabRemoved(int index)
{
    moveButton();
}

void ConsoleTabBar::moveButton(void)
{
    int y = tabRect(lastTabIndex()).bottom();
    qDebug() << y;
    m_btn->move(4, y + 1);
}


void ConsoleTabBar::showEvent(QShowEvent *event)
{
    qDebug() << "show";
    //tabButton(lastTabIndex(), QTabBar::RightSide)->resize(0, 0);
    moveButton();
    QTabBar::showEvent(event);
}
#if 0
void ConsoleTabBar::mouseReleaseEvent(QMouseEvent * event)
{
#if 0
     const QPoint pos = event->pos();

     if (tabAt(pos) == lastTabIndex() && lastTabIndex() == m_selectedIndex)
     {
         emit addButtonClicked();
         return;
     }
     if (tabAt(pos) != m_selectedIndex)
     {
         setCurrentIndex(m_prevIndex);
         qDebug() << "FOO";
     }

     m_selectedIndex = -1;
#endif
     QTabBar::mouseReleaseEvent(event);
}

void ConsoleTabBar::mouseMoveEvent(QMouseEvent* event)
{
#if 0
    const QPoint pos = event->pos();
   // qDebug() << tabButton(m_selectedIndex,QTabBar::RightSide)->pos();
    qDebug() << event->pos();
    const int max = tabRect(lastTabIndex()).y();
    if (event->pos().y() > max) // FIXME
    {
        event->ignore();
        return;
    }

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
#if 0
    m_prevIndex = currentIndex();
    m_selectedIndex = tabAt(event->pos());
    QTabBar::mousePressEvent(event);
#endif
}
#endif
int ConsoleTabBar::lastTabIndex(void)
{
    return count()-1;
}

// EOF <stefan@scheler.com>
