#include <cstdio>
#include <QMessageBox>
#include <QSerialPort>
#include <QPushButton>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "consoletab.h"
#include "consoleview.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::CMainWindow)
{
    m_ui->setupUi(this);
    //m_ui->statusBar->showMessage("Ready");
}


CMainWindow::~CMainWindow()
{
    delete m_ui;
}

void CMainWindow::showAbout(void)
{
    QMessageBox::about(this, tr("About"),
                 tr("<p>The <b>Scribble</b> example shows how to use QCMainWindow as the "
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

QSize	CMainWindow::sizeHint() const
{
    return QSize(800, 600);
}

// EOF <stefan@scheler.com>
