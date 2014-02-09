#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::CMainWindow)
{
    m_ui->setupUi(this);
}

CMainWindow::~CMainWindow()
{
    delete m_ui;
}

QSize CMainWindow::sizeHint() const
{
    return QSize(800, 600);
}

// EOF <stefan@scheler.com>
