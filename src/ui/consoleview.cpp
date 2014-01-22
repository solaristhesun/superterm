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
    ui->setupUi(this);

    connect(m_port, SIGNAL(readyRead()), this, SLOT(showData()));
}

ConsoleView::~ConsoleView()
{
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
    //append(QString(data));
    std::cout << "NEW DATA" << std::endl;
    QString oldText = toPlainText();
    setPlainText(oldText + QString(data));
#if 0
    QTextCursor c = textCursor();
    c.movePosition(QTextCursor::End);
    setTextCursor(c);
#endif
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}
