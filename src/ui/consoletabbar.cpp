#include <QDebug>
#include <QMouseEvent>
#include <QShowEvent>
#include <QPushButton>
#include <QRect>

#include "consoletabbar.h"

CConsoleTabBar::CConsoleTabBar(QWidget *parent) :
    QTabBar(parent),
    m_btn(new QPushButton(this)),
    m_selectedIndex(-1),
    m_prevIndex(-1)
{
    setFocusPolicy(Qt::NoFocus);
    setMovable(false);
    m_btn->setStyleSheet("QPushButton { border: 1px solid #cacaca; border-right: 0; border-top-left-radius: 2px; border-bottom-left-radius: 2px;margin: 1px;     background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #E1E1E1, stop: 0.4 #DDDDdd, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } QPushButton:hover, QPushButton:pressed { background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #fafafa, stop: 0.4 #f4f4f4, stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);} ");
    m_btn->setGeometry(QRect(0, 200, 27, 27 ));
    m_btn->setFocusPolicy(Qt::NoFocus);
    m_btn->setIcon(QIcon(":/icons/application_add_16x16.png"));
    connect(m_btn, SIGNAL(clicked()), this, SIGNAL(addButtonClicked()));
    // add empty tab
    //addTab("");
    moveButton();
}


void	CConsoleTabBar::tabInserted(int index)
{
    moveButton();
}

void	CConsoleTabBar::tabRemoved(int index)
{
    moveButton();
}

void CConsoleTabBar::moveButton(void)
{
    const int iLastTabIndex = count() - 1;

    if (iLastTabIndex != -1)
    {
        QRect rect = tabRect(iLastTabIndex);
        int y = rect.bottom();
        m_btn->move(rect.bottomRight().x()-28+3, y);
    }
}

void CConsoleTabBar::showEvent(QShowEvent *event)
{
    moveButton();
    QTabBar::showEvent(event);
}

void CConsoleTabBar::mouseReleaseEvent(QMouseEvent * event)
{
     QTabBar::mouseReleaseEvent(event);
}

void CConsoleTabBar::mouseMoveEvent(QMouseEvent* event)
{
    QTabBar::mouseMoveEvent(event);
}

void  CConsoleTabBar::mousePressEvent(QMouseEvent * event)
{
    QTabBar::mousePressEvent(event);
}

// EOF <stefan@scheler.com>
