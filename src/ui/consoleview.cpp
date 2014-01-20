
#include <iostream>
#include <QSerialPort>

#include "consoleview.h"
#include "ui_consoleview.h"

ConsoleView::ConsoleView(QSerialPort *port, QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::ConsoleView),
    m_port(port)
{
    ui->setupUi(this);

    connect(m_port, SIGNAL(readyRead()), this, SLOT(showData()));
}

ConsoleView::~ConsoleView()
{
    delete ui;
}

void ConsoleView::keyPressEvent(QKeyEvent *)
{
    std::cout << "KEY" << std::endl;
}

void ConsoleView::showData()
{
    QByteArray data = m_port->readAll();
    append(QString(data));
    std::cout << "NEW DATA" << std::endl;
}
