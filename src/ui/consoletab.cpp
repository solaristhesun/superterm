#include <QSerialPort>
#include <QDebug>
#include <iostream>
#include <cstdio>

#include "consoletab.h"
#include "ui_consoletab.h"
#include "consoletabwidget.h"

ConsoleTab::ConsoleTab(QTabWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleTab),
    m_parent(parent),
    m_port(NULL),
    m_lastTabIndex(0)
{
    ui->setupUi(this);
}

ConsoleTab::~ConsoleTab()
{
    delete ui;
    delete m_port;
}


void ConsoleTab::toggleFullScreen(void)
{
    if (!isFullScreen())
    {
        m_lastTabIndex = m_parent->currentIndex();
        setParent(0);
        showFullScreen();
    }
    else
    {
        setParent(m_parent);
        m_parent->insertTab(m_lastTabIndex, this, "foo");
        m_parent->setCurrentIndex(m_lastTabIndex);
        ui->textEdit->setFocus();
    }
}

void ConsoleTab::onConnectClicked(void)
{
    ui->btnBar->hide();
    m_port = new QSerialPort(ui->comboBox->currentText());
    if (m_port->open(QIODevice::ReadWrite)){
        puts("PORT OPEN");
    }
    else
    {
        qDebug() << ui->comboBox->currentText();
    }
    connect(m_port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));

    ui->textEdit->setEnabled(true);
    ui->textEdit->setFocus();
}

void ConsoleTab::onDataAvailable(void)
{
    QByteArray data = m_port->readAll();
    for (int p = 0; p < data.size(); p++)
    {
        printf("0x%02x ", data.at(p));
    }
    printf("\n");

    QString str = data;
    std::cout << "NEW DATA [" << str.toStdString() << "]" << std::endl;
    str = str.replace("\r", "<br>");
    str = str.replace(" ", "&nbsp;");

    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->textCursor().insertHtml(str);
    ui->textEdit->moveCursor(QTextCursor::End);
}

void ConsoleTab::onKeyPressed(QString text)
{
    puts("KEY");
    QByteArray data;
    data.append(text);
    m_port->write(data);
}
