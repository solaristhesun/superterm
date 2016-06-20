#include <QMessageBox>
#include <QDir>
#include <QDebug>

#include "globals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "com/serialconnection.h"

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
            qDebug() << str;
            QFile file(QCoreApplication::applicationDirPath() + "/" + str);
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);
            CSerialConnection con;
            in >> con;
            qDebug() << con.getName();

            m_ui->tabWidget->addNewTab(con.createSerialPort());

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
