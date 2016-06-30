#include <QMessageBox>
#include <QDir>
#include <QMoveEvent>
#include <QDebug>
#include <QRect>

#include "globals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "obj/session.h"
#include "consoletab.h"
#include "consoletabbar.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::CMainWindow)
{
    m_ui->setupUi(this);

    setWindowTitle(g_sAppFullName);
}

CMainWindow::~CMainWindow()
{
    delete m_ui;
}

QSize CMainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void CMainWindow::aboutToQuit(void)
{
    removeTabFiles();
    m_ui->tabWidget->aboutToQuit();
}

void CMainWindow::addExistingTabsFromFile(void)
{
    QDir dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList()<<"*.con", QDir::Files);

    if (!files.isEmpty())
    {
        m_ui->tabWidget->clear(); // FIXME: incorrect, does not destroy initial tab
        foreach(const QString &str, files)
        {
            QFile file(QCoreApplication::applicationDirPath() + "/" + str);
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);

            CSession* session = new CSession();
            in >> *session;
            qDebug() << "adding tab" << session->getDeviceName();

            m_ui->tabWidget->addNewTab(session);

            file.close();
        }
    }
}

void CMainWindow::removeTabFiles(void)
{
    QDir dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList()<<"*.con", QDir::Files);

    if (!files.isEmpty())
    {
        foreach(const QString &str, files)
        {
            dir.remove(str);
        }
    }
}

void CMainWindow::addTab(QWidget* widget, QString& tabText)
{
    m_ui->tabWidget->destroyTab(0); // FIXME: bad design, why add tab in the first place?
    m_ui->tabWidget->addTab(widget, tabText);
}

void CMainWindow::moveEvent(QMoveEvent * event)
{
    // check if mainwindow is dropped in other mainwindow's tab bar
    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        if (widget->objectName() == "CMainWindow" && widget != this)
        {
            CMainWindow *w = static_cast<CMainWindow*>(widget);
            CConsoleTabBar *tabBar = static_cast<CConsoleTabBar*>(w->m_ui->tabWidget->tabBar());
            QRect globalWidgetsRect = QRect(tabBar->mapToGlobal(QPoint(0,0)), tabBar->size());

            if (globalWidgetsRect.contains(event->pos() + tabBar->getClickOffset()))
            {
                qDebug() << "DROPBABLE";

                // remove tab from current mainwindow
                CConsoleTab *tab = static_cast<CConsoleTab*>(m_ui->tabWidget->currentWidget());
                QString tabText = m_ui->tabWidget->tabText(m_ui->tabWidget->currentIndex());
                m_ui->tabWidget->removeTab(m_ui->tabWidget->currentIndex());

                // move tab to other mainwindow
                w->m_ui->tabWidget->addTab(tab, tabText);
                w->m_ui->tabWidget->setCurrentWidget(tab);

                // destroy current main window
                hide();
                //deleteLater(); // FIXME: correct?
            }
        }

    }
}

// EOF <stefan@scheler.com>
