#include <QColorDialog>
#include <QSerialPort>
#include <QDebug>
#include <iostream>
#include <QMenu>
#include <QMouseEvent>
#include <cstdio>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFontDialog>
#include <QSettings>

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

    fillComboBoxes();

    // load font from settings
    QSettings settings;
    QFont consoleFont;
    QColor color = QColor(settings.value("background").toString());
    consoleFont.fromString(settings.value("font").toString());
    setConsoleFont(consoleFont);
    setBackgroundColor(color);
}

ConsoleTab::~ConsoleTab()
{
    delete m_ui;
    delete m_port;
}

void ConsoleTab::fillComboBoxes(void)
{
    QComboBox *combo = NULL;

    // fill baud rates
    combo = m_ui->comboBaudRates;
#if 0
    foreach (const qint32 baudrate, QSerialPortInfo::standardBaudRates())
    {
        combo->addItem(QString::number(baudrate), QVariant(baudrate));
    }
#endif
    // just use more common baud rates for now
    combo->addItem("9600", QVariant(9600));
    combo->addItem("19200", QVariant(19200));
    combo->addItem("38400", QVariant(38400));
    combo->addItem("57600", QVariant(57600));
    combo->addItem("115200", QVariant(115200));
    combo->addItem("custom", QVariant(0));


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
    combo->setCurrentIndex(0);
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

void ConsoleTab::showContextMenu(const QPoint &pt)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(m_ui->actionConfiguration);
    menu->addAction(m_ui->actionChangeColor);
    menu->addSeparator();
    menu->addAction(m_ui->actionChangeFont);
    menu->addSeparator();
    menu->addAction(m_ui->actionClear);
    menu->addSeparator();
    menu->addAction(m_ui->actionAbout);
    menu->exec(mapToGlobal(pt));
    delete menu;
}

void ConsoleTab::showConnectBar(void)
{
    m_ui->btnBar->show();
}

void ConsoleTab::showColorDialog(void)
{
    QSettings settings;
    QColor initial(settings.value("background").toString());
    QColor rgb = QColorDialog::getColor(initial, this);
    if (rgb.isValid())
    {
        setBackgroundColor(rgb);
        settings.setValue("background", rgb.name());
    }
}

void ConsoleTab::showFontDialog(void)
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, m_ui->consoleView->font(), this);
    if (ok)
    {
        QSettings settings;
        settings.setValue("font", font.toString());
        setConsoleFont(font);
    }
}

void ConsoleTab::setConsoleFont(const QFont &font)
{
    m_ui->consoleView->setFont(font);
    m_ui->consoleView->refreshCursor();
}

void ConsoleTab::setBackgroundColor(const QColor &color)
{
    setStyleSheet(QString("QPlainTextEdit { background-color: %1; }").arg(color.name()));
}

void ConsoleTab::onConnectClicked(void)
{
    const QString portName = m_ui->comboPorts->currentData().toString();

    if (!m_port)
    {
        m_port = new QSerialPort(portName);

        if (m_port->open(QIODevice::ReadWrite))
        {
            // configure serial port

            m_port->setBaudRate(m_ui->comboBaudRates->currentData().toInt());
            m_port->setDataBits(m_ui->comboDataBits->currentData().value<QSerialPort::DataBits>());
            m_port->setParity(m_ui->comboParity->currentData().value<QSerialPort::Parity>());
            m_port->setStopBits(m_ui->comboStopBits->currentData().value<QSerialPort::StopBits>());
            m_port->setFlowControl(QSerialPort::NoFlowControl);

            m_port->clear(QSerialPort::AllDirections);

            connect(m_port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));

            m_ui->btnBar->hide();
            m_ui->consoleView->setEnabled(true);
            m_ui->consoleView->setFocus();
            m_parent->setCurrentTabText(portName);

            m_ui->comboPorts->setEnabled(false);
            m_ui->btnConnect->setText("&Disconnect");

            m_ui->statusBar->showMessage(tr("Successfully connected to %1.").arg(portName), 3000);

        }
        else
        {
            qDebug() << m_port->error();
            delete m_port;
            m_port = NULL;
            m_ui->statusBar->showMessage("Error. Failed to open port.");
            //QMessageBox::critical(this, "Error", "Failed to open port", QMessageBox::Ok, QMessageBox::NoButton);
        }

        connect(m_port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(showError(QSerialPort::SerialPortError)));
    }
    else
    {
        m_port->disconnect();
        delete m_port;
        m_port = NULL;
        m_ui->comboPorts->setEnabled(true);
        m_ui->btnConnect->setText("&Connect");
        m_ui->consoleView->setEnabled(false);
    }
}

void ConsoleTab::showError(QSerialPort::SerialPortError error)
{
    qDebug() << "ERROR: " << error;
    m_port->close();
    m_ui->statusBar->showMessage("ERROR");
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
#if 0
    for (int p = 0; p < data.size(); p++)
    {
        printf("0x%02x ", data.at(p));
    }
    printf("\n");
#endif
    QString str = data;
    //std::cout << "NEW DATA [" << str.toStdString() << "]" << std::endl;
    str = str.replace("\r\n", "\n");
    str = str.replace("\r", "\n");
    //str = str.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
#if 1
    //highlight.cursor = m_ui->consoleView->textCursor();

#endif

    m_ui->consoleView->insertPlainText(str);
}

void ConsoleTab::onKeyPressed(QString text)
{
    if (text.isEmpty())
        return;

    QByteArray data;
    data.append(text.toLatin1());
    printf("KEY [%s] (0x%02x, 0x%02x)\n", text.toLatin1().constData(), text.toLatin1().constData()[0], data.constData()[0]);
    m_port->write(data.constData());
}

// EOF <stefan@scheler.com>
