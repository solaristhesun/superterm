#include <QSerialPort>
#include <QDebug>
#include <iostream>
#include <QMenu>
#include <QMouseEvent>
#include <cstdio>
#include <QSerialPortInfo>
#include <QMessageBox>

#include "consoletab.h"
#include "ui_consoletab.h"
#include "consoletabwidget.h"

quint32   ConsoleTab::m_u32counter = 1;

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

ConsoleTab::ConsoleTab(ConsoleTabWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ConsoleTab),
    m_parent(parent),
    m_port(NULL),
    m_lastTabIndex(0)
{
    m_ui->setupUi(this);

    // fill port combo box
    refreshPorts();
    fillComboBoxes();
}

ConsoleTab::~ConsoleTab()
{
    delete m_ui;
    delete m_port;
}

void ConsoleTab::refreshPorts(void)
{
    m_ui->comboPorts->clear();
    m_ui->comboPorts->addItem(tr("Select port"));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        const QString title = QString("%1 (%2) [%3]").arg(info.portName(), info.description(), info.manufacturer());
        m_ui->comboPorts->addItem(title, QVariant(info.portName()));
        qDebug() << info.systemLocation();
        qDebug() << info.vendorIdentifier();
        qDebug() << info.productIdentifier();
    }

    m_ui->comboPorts->addItem("/dev/ttyS10", QVariant("/dev/ttyS10"));
    m_ui->comboPorts->addItem("/dev/ttyS11", QVariant("/dev/ttyS11"));
}

void ConsoleTab::fillComboBoxes(void)
{
    QComboBox *combo = NULL;

    // fill baud rates
    combo = m_ui->comboBaudRates;
    foreach (const qint32 baudrate, QSerialPortInfo::standardBaudRates())
    {
        combo->addItem(QString::number(baudrate), QVariant(baudrate));
    }

    // fill data bits
    combo = m_ui->comboDataBits;
    combo->clear();
    combo->addItem("5", QVariant(QSerialPort::Data5));
    combo->addItem("6", QVariant(QSerialPort::Data6));
    combo->addItem("7", QVariant(QSerialPort::Data7));
    combo->addItem("8", QVariant(QSerialPort::Data8));
    combo->setCurrentIndex(3);

    // fill parity
    combo = m_ui->comboParity;
    combo->clear();
    combo->addItem(tr("None"), QVariant(QSerialPort::NoParity));
    combo->addItem(tr("Even"), QVariant(QSerialPort::EvenParity));
    combo->addItem(tr("Odd"), QVariant(QSerialPort::OddParity));
    combo->addItem(tr("Space"), QVariant(QSerialPort::SpaceParity));
    combo->addItem(tr("Mark"), QVariant(QSerialPort::MarkParity));

    // fill stop bits
    combo = m_ui->comboStopBits;
    combo->clear();
    combo->addItem("1", QVariant(QSerialPort::OneStop));
    combo->addItem("1.5", QVariant(QSerialPort::OneAndHalfStop));
    combo->addItem("2", QVariant(QSerialPort::TwoStop));
    combo->setCurrentIndex(0);

    // fill flow control
    combo = m_ui->comboFlowControl;
    combo->clear();
    combo->addItem(tr("None"), QVariant(QSerialPort::NoFlowControl));
    combo->addItem(tr("Xon/Xoff"), QVariant(QSerialPort::SoftwareControl));
    combo->addItem(tr("Hardware"), QVariant(QSerialPort::HardwareControl));
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
        m_ui->consoleView->setFocus();
    }
}

void ConsoleTab::onConnectClicked(void)
{
    const QString portName = m_ui->comboPorts->currentData().toString();

    m_port = new QSerialPort(portName);

    if (m_port->open(QIODevice::ReadWrite))
    {
        // configure serial port

        m_port->setBaudRate(m_ui->comboBaudRates->currentData().toInt());
        m_port->setDataBits(m_ui->comboDataBits->currentData().value<QSerialPort::DataBits>());
        m_port->setParity(m_ui->comboParity->currentData().value<QSerialPort::Parity>());
        m_port->setStopBits(m_ui->comboStopBits->currentData().value<QSerialPort::StopBits>());
        m_port->setFlowControl(m_ui->comboFlowControl->currentData().value<QSerialPort::FlowControl>());

        connect(m_port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));

        m_parent->addTab(new ConsoleTab(m_parent));

        m_ui->btnBar->hide();
        m_ui->consoleView->setEnabled(true);
        m_ui->consoleView->setFocus();
        m_parent->setCurrentTabText(portName);
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to open port", QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void ConsoleTab::onComboChanged(void)
{
    if (m_ui->comboPorts->currentIndex() != 0 && m_ui->comboBaudRates->currentIndex() != 0)
    {
        m_ui->btnConnect->setEnabled(true);
    }
    else
    {
        m_ui->btnConnect->setEnabled(false);
    }
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

    m_ui->consoleView->moveCursor(QTextCursor::End);
    m_ui->consoleView->textCursor().insertHtml(str);
    m_ui->consoleView->moveCursor(QTextCursor::End);
}

void ConsoleTab::onKeyPressed(QString text)
{
    puts("KEY");
    QByteArray data;
    data.append(text);
    m_port->write(data);
}

// EOF <stefan@scheler.com>
