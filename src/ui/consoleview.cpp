#include <QScrollBar>
#include <iostream>
#include <QSerialPort>
#include <QKeyEvent>

#include "consoleview.h"
#include "ui_consoleview.h"

ConsoleView::ConsoleView(QSerialPort *port, QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::ConsoleView),
    m_port(port)
{
    puts("CONSTRUCTOR");
    ui->setupUi(this);

    connect(m_port, SIGNAL(readyRead()), this, SLOT(showData()));
}

ConsoleView::~ConsoleView()
{
    puts("DESSTRUCTOR");
    delete m_port;
    delete ui;
}

void ConsoleView::keyPressEvent(QKeyEvent *e)
{
    std::cout << "KEY" << std::endl;
    QByteArray data;
    data.append(e->text());
    m_port->write(data);
}

void ConsoleView::showData()
{
    QByteArray data = m_port->readAll();
    for (int p = 0; p < data.size(); p++)
    {
        printf("0x%02x ", data.at(p));
    }
    printf("\n");
    //append(QString(data));
    QString str = data;
    std::cout << "NEW DATA [" << str.toStdString() << "]" << std::endl;
    str = str.replace("\r", "<br>");
    str = str.replace(" ", "&nbsp;");

    moveCursor(QTextCursor::End);
    textCursor().insertHtml(str);
    moveCursor(QTextCursor::End);

    scrollDown();
}

void ConsoleView::scrollDown(void)
{
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

// EOF <stefan@scheler.com>
