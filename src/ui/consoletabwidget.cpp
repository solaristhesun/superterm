#include <QTabBar>
#include <QApplication>
#include <QDebug>

#include "consoletabwidget.h"
#include "consoletabbar.h"
#include "consoletab.h"
#include "enumerator/portenumerator.h"
#include "obj/session.h"

CConsoleTabWidget::CConsoleTabWidget(QWidget *parent) :
    QTabWidget(parent),
    m_tabBar(new CConsoleTabBar(this)),
    m_pe(new CPortEnumerator())
{
    setTabBar(m_tabBar);
    addNewTab(NULL);

    connect(m_tabBar, SIGNAL(addButtonClicked()), this, SLOT(handleAddButtonClicked()));
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

void CConsoleTabWidget::handleAddButtonClicked(void)
{
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

void CConsoleTabWidget::aboutToQuit(void)
{
    emit appQuits();
}

// EOF <stefan@scheler.com>
