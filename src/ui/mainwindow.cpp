#include <QMessageBox>
#include <QDir>
#include <QDebug>

#include "globals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "obj/session.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::CMainWindow)
{
    m_ui->setupUi(this);
    addExistingTabsFromFile();

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
        m_ui->tabWidget->clear();
        foreach(const QString &str, files)
        {
            qDebug() << "FILENAME:" << str;
            QFile file(QCoreApplication::applicationDirPath() + "/" + str);
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);

            CSession* session = new CSession();
            in >> *session;
            qDebug() << "adding tab " << session->getDeviceName();

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

// EOF <stefan@scheler.com>
