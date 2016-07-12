#include <QMessageBox>
#include <QDir>
#include <QMoveEvent>
#include <QDebug>
#include <QRect>

#include "ui_mainwindow.h"
#include "ui/mainwindow.h"
#include "ui/consoletab.h"
#include "ui/consoletabbar.h"
#include "ui/consoletabfactory.h"
#include "misc/globals.h"
#include "session/session.h"

CMainWindow::CMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::CMainWindow)
{
    qDebug() << "CMainWindow::CMainWindow()";
    m_ui->setupUi(this);

    QObject::connect(qApp, &QApplication::aboutToQuit, this, &CMainWindow::aboutToQuit);

    setWindowTitle(g_sAppFullName);
}

CMainWindow::~CMainWindow()
{
    qDebug() << "CMainWindow::~CMainWindow()" << this;
    delete m_ui;
}

QSize CMainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void CMainWindow::aboutToQuit()
{
    removeTabFiles();
    m_ui->tabWidget->aboutToQuit();
}

void CMainWindow::addExistingTabsFromFile()
{
    QDir        dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList() << "*.con", QDir::Files);

    if (!files.isEmpty())
    {
        m_ui->tabWidget->clear(); // FIXME: incorrect, does not destroy initial tab
        for (const QString& str: files)
        {
            QFile file(QCoreApplication::applicationDirPath() + "/" + str);
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);

            CSession* session = new CSession();
            in >> *session;
            qDebug() << "adding tab" << session->getDeviceName();

            m_ui->tabWidget->addTab(CConsoleTabFactory::createTabFromSession(session));

            file.close();
        }
    }
    else
    {
        m_ui->tabWidget->addTab(CConsoleTabFactory::createTab());
    }
}

void CMainWindow::removeTabFiles()
{
    QDir        dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList() << "*.con", QDir::Files);

    if (!files.isEmpty())
    {
        for (const QString& str: files)
        {
            dir.remove(str);
        }
    }
}

void CMainWindow::attachTab(CConsoleTab* tab)
{
    qDebug() << "attaching tab" << tab->getLabel() << "to" << this;

    m_ui->tabWidget->addTab(tab);
}

CConsoleTab* CMainWindow::detachTab()
{
    int curIndex = m_ui->tabWidget->currentIndex();

    CConsoleTab* tab = m_ui->tabWidget->widget(curIndex);
    m_ui->tabWidget->removeTab(curIndex);

    qDebug() << "detaching tab" << tab->getLabel() << "from" << this;

    return tab;
}

int CMainWindow::getTabCount() const
{
    return m_ui->tabWidget->count();
}

QRect CMainWindow::getTabBarRect() const
{
    CConsoleTabBar* tabBar = static_cast<CConsoleTabBar*>(m_ui->tabWidget->tabBar());
    return QRect(tabBar->mapToGlobal(QPoint(0, 0)), tabBar->size());
}

// EOF <stefan@scheler.com>
