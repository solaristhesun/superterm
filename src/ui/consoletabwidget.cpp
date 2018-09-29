#include <QTabBar>
#include <QApplication>
#include <QDebug>
#include <QMenu>

#include "ui/consoletabwidget.h"
#include "ui/consoletabbar.h"
#include "ui/consoletab.h"
#include "ui/consoletabfactory.h"
#include "ui/consoletab.h"
#include "ui/mainwindow.h"
#include "serial/portenumerator.h"
#include "session/session.h"

ConsoleTabWidget::ConsoleTabWidget(QWidget* parent)
    : QTabWidget(parent)
    , m_tabBar(new ConsoleTabBar(this))
    , m_contextMenu(nullptr)
    , m_renameTabAction(new QAction(tr("Rename tab..."), this))
{
    qDebug() << "CConsoleTabWidget::CConsoleTabWidget()";

    QTabWidget::setTabBar(m_tabBar);

    createContextMenu();

    connect(m_tabBar, &ConsoleTabBar::addButtonClicked, this, &ConsoleTabWidget::onAddButtonClicked);
    connect(m_tabBar, &ConsoleTabBar::tabDetached, this, &ConsoleTabWidget::onTabDetached);
    connect(m_tabBar, &ConsoleTabBar::customContextMenuRequested, this, &ConsoleTabWidget::showContextMenu);
    connect(m_renameTabAction, &QAction::triggered, this, &ConsoleTabWidget::showTabRenameDialog);
    connect(this, &ConsoleTabWidget::tabCloseRequested, this, &ConsoleTabWidget::onTabCloseRequested);
}

void ConsoleTabWidget::showContextMenu(const QPoint& pt)
{
    if (m_tabBar->tabAt(pt) == currentIndex())
    {
        m_contextMenu->exec(mapToGlobal(pt));
    }
}

ConsoleTabWidget::~ConsoleTabWidget()
{
    qDebug() << "CConsoleTabWidget::~CConsoleTabWidget()";
    delete m_tabBar;
}

ConsoleTab* ConsoleTabWidget::currentWidget() const
{
    return static_cast<ConsoleTab*>(QTabWidget::currentWidget());
}

ConsoleTab* ConsoleTabWidget::widget(int index) const
{
    return static_cast<ConsoleTab*>(QTabWidget::widget(index));
}

void ConsoleTabWidget::destroyTab(int index)
{
    QWidget* tab = QTabWidget::widget(index);

    // remove tab from widget
    QTabWidget::removeTab(index);

    // delete tab
    if (tab)
    {
        delete tab;
    }
}

void ConsoleTabWidget::onTabCloseRequested(int index)
{
    ConsoleTab* tab = widget(index);
    tab->disconnectEndpoint();
}

void ConsoleTabWidget::closeTab(int index)
{
    destroyTab(index);

    // select next tab or close window
    if (QTabWidget::count() > 0)
    {
        QTabWidget::setCurrentIndex(index - 1);
    }
    else
    {
        QApplication::activeWindow()->close();
    }
}


void ConsoleTabWidget::activateNextTab()
{
    int currentIndex = QTabWidget::currentIndex();
    int maximumIndex = QTabWidget::count() - 1;

    if (currentIndex < maximumIndex)
    {
        currentIndex++;
    }
    else
    {
        currentIndex = 0;
    }

    QTabWidget::setCurrentIndex(currentIndex);
}

void ConsoleTabWidget::activatePrevTab()
{
    int currentIndex = QTabWidget::currentIndex();
    int maximumIndex = QTabWidget::count() - 1;

    if (currentIndex > 0)
    {
        currentIndex--;
    }
    else
    {
        currentIndex = maximumIndex;
    }

    QTabWidget::setCurrentIndex(currentIndex);
}

void ConsoleTabWidget::onTabDetached(int index)
{
    qDebug() << "[slot] onTabDetached";

    ConsoleTab* tab = widget(index);

    if (tab)
    {
        QTabWidget::removeTab(index);

        QObject::disconnect(tab, &ConsoleTab::labelChanged, this, &ConsoleTabWidget::setCurrentTabText);

        // add tab to new window
        MainWindow* newWin = m_tabBar->getNewMainWindow();
        newWin->attachTab(tab);
    }
}

void ConsoleTabWidget::onAddButtonClicked()
{
    qDebug() << "[slot] onAddButtonClicked";

    addTab(ConsoleTabFactory::createTab());
}

void ConsoleTabWidget::addTab(ConsoleTab* tab)
{
    qDebug() << "adding tab" << tab->getLabel();

    QTabWidget::addTab(tab, tab->getLabel());
    QTabWidget::setCurrentWidget(tab);

    QObject::connect(tab, &ConsoleTab::labelChanged, this, &ConsoleTabWidget::setCurrentTabText);

    m_tabBar->moveButton();
}

void ConsoleTabWidget::setConsoleFont(const QFont& font)
{
    // set font on all tabs
    for (int i = 0; i < QTabWidget::count(); i++)
    {
        ConsoleTab* tab = widget(i);
        tab->setConsoleFont(font);
    }
}

void ConsoleTabWidget::setCurrentTabText(const QString& text)
{
    const int curIndex = currentIndex();
    setTabText(curIndex, text);
    m_tabBar->moveButton();
}


void ConsoleTabWidget::createContextMenu()
{
    m_renameTabAction->setShortcut(Qt::Key_Control | Qt::Key_R);

    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(m_renameTabAction);
}

void ConsoleTabWidget::showTabRenameDialog()
{
    ConsoleTab* tab = currentWidget();
    tab->showRenameTabDialog();
}

// EOF <stefan@scheler.com>
