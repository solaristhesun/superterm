#include <cstdio>
#include <QMessageBox>
#include <QSerialPort>

#include "mainwindow.h"
#include "newconnectiondialog.h"
#include "ui_mainwindow.h"
#include "consoleview.h"
#include "../consolemgr.h"

MainWindow::MainWindow(ConsoleMgr& consoleMgr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_consoleMgr(consoleMgr)
{
    QSerialPort *port = new QSerialPort("/dev/ttyS10");
    if (port->open(QIODevice::ReadWrite)){
        puts("PORT OPEN");
    }
    ui->setupUi(this);
    ui->tabWidget->insertTab(0, new ConsoleView(port, this), "TG+ Modul");
    ui->tabWidget->setCurrentIndex(0);

    ui->statusBar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout(void)
{
    QMessageBox::about(this, tr("About"),
                 tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
                    "base widget for an application, and how to reimplement some of "
                    "QWidget's event handlers to receive the events generated for "
                    "the application's widgets:</p><p> We reimplement the mouse event "
                    "handlers to facilitate drawing, the paint event handler to "
                    "update the application and the resize event handler to optimize "
                    "the application's appearance. In addition we reimplement the "
                    "close event handler to intercept the close events before "
                    "terminating the application.</p><p> The example also demonstrates "
                    "how to use QPainter to draw an image in real time, as well as "
                    "to repaint widgets.</p>"));
}


void MainWindow::connectPort()
{
    puts("CONNECT");
#if 0
    NewConnectionDialog dialog;
    dialog.exec();
    ui->tabWidget->insertTab(-1, new ConsoleView(), "SpaceControl");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
#endif
}

void MainWindow::closeTab()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

// EOF <stefan@scheler.com>
