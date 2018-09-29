#include <QDir>
#include <QDebug>
#include <QSerialPort>

#include "src/session/session.h"
#include "src/misc/globals.h"
#include "connectionbar.h"
#include "ui_connectionbar.h"

ConnectionBar::ConnectionBar(QWidget* parent)
    : QFrame(parent)
    , m_ui(new Ui::ConnectionBar)
{
    m_ui->setupUi(this);

    fillComboBoxes();

    m_ui->lineEditBaudRate->setText("9600");
    m_ui->lineEditBaudRate->hide();
    m_ui->comboDataBits->hide();
    m_ui->comboFlowControl->hide();
    m_ui->comboParity->hide();
    m_ui->comboStopBits->hide();
    m_ui->comboConfigurations->hide();
    m_ui->btnSave->hide(); // save feature for now

    connect(m_ui->btnConnect, &QPushButton::clicked, this, &ConnectionBar::connectClicked);
    connect(m_ui->btnSave, &QPushButton::clicked, this, &ConnectionBar::saveClicked);
    connect(m_ui->comboConfigurations, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated), this, &ConnectionBar::configurationChanged);
}

ConnectionBar::~ConnectionBar()
{
    delete m_ui;
}

void ConnectionBar::showEvent(QShowEvent* event)
{
    QDir        dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList() << "*.xml", QDir::Files);
    m_ui->comboConfigurations->clear();
    m_ui->comboConfigurations->insertItems(0, files);

    if (!files.isEmpty())
    {
        m_ui->comboConfigurations->insertItem(0, tr("Select configuration"));
        m_ui->comboConfigurations->setCurrentIndex(0);
        m_ui->comboConfigurations->show();
    }

    QWidget::showEvent(event);
}

void ConnectionBar::setPortEnumerator(PortEnumerator* pe)
{
    m_ui->comboPorts->setPortEnumerator(pe);
}

QString ConnectionBar::getDeviceName() const
{
    return m_ui->comboPorts->currentPortInfo().portName();
}

void ConnectionBar::setDeviceName(const QString& deviceName)
{
    m_ui->comboPorts->setCurrentDeviceName(deviceName);
}

QString ConnectionBar::getDeviceDesc() const
{
    return m_ui->comboPorts->currentPortInfo().description();
}

QString ConnectionBar::getBaudRate() const
{
    if (m_ui->lineEditBaudRate->isVisible())
    {
        return m_ui->lineEditBaudRate->text();
    }
    else
    {
        return m_ui->comboBaudRates->currentText();
    }
}

void ConnectionBar::setBaudRate(const QString& baudRate)
{
    if (m_ui->lineEditBaudRate->isVisible())
    {
        m_ui->comboBaudRates->setCurrentText("custom");
        m_ui->lineEditBaudRate->setText(baudRate);
    }
    else
    {
        m_ui->comboBaudRates->setCurrentText(baudRate);
    }
}

QString ConnectionBar::getDataBits() const
{
    return m_ui->comboDataBits->currentText();
}

void ConnectionBar::setDataBits(const QString& dataBits)
{
    m_ui->comboDataBits->setCurrentText(dataBits);
}

QString ConnectionBar::getParity() const
{
    return m_ui->comboParity->currentText();
}

void ConnectionBar::setParity(const QString& parity)
{
    m_ui->comboParity->setCurrentText(parity);
}

QString ConnectionBar::getStopBits() const
{
    return m_ui->comboStopBits->currentText();
}

void ConnectionBar::setStopBits(const QString& stopBits)
{
    m_ui->comboStopBits->setCurrentText(stopBits);
}

QString ConnectionBar::getFlowControl() const
{
    return m_ui->comboFlowControl->currentText();
}

void ConnectionBar::setFlowControl(const QString& flowControl)
{
    m_ui->comboFlowControl->setCurrentText(flowControl);
}

void ConnectionBar::loadFromSession(Session* session)
{
    qDebug() << *session << session->getDeviceDesc();
    m_ui->comboPorts->setPort(session->getDeviceName());

    if (m_ui->comboBaudRates->findText(QString::number(session->getBaudRate())) >= 0)
    {
        m_ui->comboBaudRates->setCurrentText(QString::number(session->getBaudRate()));
    }
    else
    {
        m_ui->comboBaudRates->setCurrentText("custom");
        m_ui->lineEditBaudRate->setText(QString::number(session->getBaudRate()));
    }

    m_ui->comboDataBits->setCurrentText(QString::number(session->getDataBits()));

    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(session->getParity());
    m_ui->comboParity->setCurrentText(Globals::ParityNameMap.value(parity));

    QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(session->getStopBits());
    m_ui->comboStopBits->setCurrentText(Globals::StopBitsNameMap.value(stopBits));

    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(session->getFlowControl());
    m_ui->comboFlowControl->setCurrentText(Globals::FlowControlNameMap.value(flowControl));

    m_ui->btnConnect->setEnabled(true); // FIXME: unsauber
}

void ConnectionBar::fillComboBoxes()
{
    QComboBox* combo = nullptr;

    /** FIXME: DO THIS IN A MORE ELEGANT WAY! */
    //while(m_pe->getAvailablePorts().count() == 0) { QThread::msleep(20); }

    // fill baud rates
    combo = m_ui->comboBaudRates;

    // just use more common baud rates for now
    combo->clear();
    combo->addItem(tr("Select baud rate"));
    combo->addItem("9600", QVariant(9600));
    combo->addItem("19200", QVariant(19200));
    combo->addItem("38400", QVariant(38400));
    combo->addItem("57600", QVariant(57600));
    combo->addItem("115200", QVariant(115200));
    combo->addItem("custom", QVariant(0));

    // fill data bits
    combo = m_ui->comboDataBits;
    combo->clear();
    combo->addItem("5", qVariantFromValue(QSerialPort::Data5));
    combo->addItem("6", qVariantFromValue(QSerialPort::Data6));
    combo->addItem("7", qVariantFromValue(QSerialPort::Data7));
    combo->addItem("8", qVariantFromValue(QSerialPort::Data8));
    combo->setCurrentIndex(3);

    // fill parity
    combo = m_ui->comboParity;
    combo->clear();
    combo->addItem(tr("None"), qVariantFromValue(QSerialPort::NoParity));
    combo->addItem(tr("Even"), qVariantFromValue(QSerialPort::EvenParity));
    combo->addItem(tr("Odd"), qVariantFromValue(QSerialPort::OddParity));
    combo->addItem(tr("Space"), qVariantFromValue(QSerialPort::SpaceParity));
    combo->addItem(tr("Mark"), qVariantFromValue(QSerialPort::MarkParity));

    // fill stop bits
    combo = m_ui->comboStopBits;
    combo->clear();
    combo->addItem("1", qVariantFromValue(QSerialPort::OneStop));
    combo->addItem("1.5", qVariantFromValue(QSerialPort::OneAndHalfStop));
    combo->addItem("2", qVariantFromValue(QSerialPort::TwoStop));
    combo->setCurrentIndex(0);

    // fill flow control
    combo = m_ui->comboFlowControl;
    combo->clear();
    combo->addItem(tr("None"), qVariantFromValue(QSerialPort::NoFlowControl));
    combo->addItem(tr("Xon/Xoff"), qVariantFromValue(QSerialPort::SoftwareControl));
    combo->addItem(tr("Hardware"), qVariantFromValue(QSerialPort::HardwareControl));
    combo->setCurrentIndex(0);
}

void ConnectionBar::onMoreClicked()
{
    if (!m_ui->comboParity->isVisible())
    {
        m_ui->comboDataBits->show();
        m_ui->comboFlowControl->show();
        m_ui->comboParity->show();
        m_ui->comboStopBits->show();
        m_ui->btnMore->setText(tr("<< &Less"));
    }
    else
    {
        m_ui->comboDataBits->hide();
        m_ui->comboFlowControl->hide();
        m_ui->comboParity->hide();
        m_ui->comboStopBits->hide();
        m_ui->btnMore->setText(tr("&More >>"));
    }
}

void ConnectionBar::onComboChanged()
{
    qDebug() << "[slot] onComboChanged";

    if (m_ui->comboPorts->currentIndex() != 0 && m_ui->comboBaudRates->currentIndex() != 0)
    {
        m_ui->btnConnect->setEnabled(true);
        m_ui->btnSave->setEnabled(true);
    }
    else
    {
        m_ui->btnConnect->setEnabled(false);
        m_ui->btnSave->setEnabled(false);
    }

    if (m_ui->comboBaudRates->currentText() == "custom")
    {
        m_ui->lineEditBaudRate->show();
    }
    else
    {
        m_ui->lineEditBaudRate->hide();
    }
}

void ConnectionBar::onDisconnected()
{
    m_ui->comboPorts->setEnabled(true);
    m_ui->lineEditBaudRate->setEnabled(true);
    m_ui->comboBaudRates->setEnabled(true);
    m_ui->comboDataBits->setEnabled(true);
    m_ui->comboFlowControl->setEnabled(true);
    m_ui->comboParity->setEnabled(true);
    m_ui->comboStopBits->setEnabled(true);
    m_ui->comboConfigurations->setEnabled(true);

    m_ui->btnConnect->setText(tr("&Connect"));

    QWidget::show();
}

void ConnectionBar::onConnected()
{
    m_ui->comboPorts->setEnabled(false);
    m_ui->lineEditBaudRate->setEnabled(false);
    m_ui->comboBaudRates->setEnabled(false);
    m_ui->comboDataBits->setEnabled(false);
    m_ui->comboFlowControl->setEnabled(false);
    m_ui->comboParity->setEnabled(false);
    m_ui->comboStopBits->setEnabled(false);
    m_ui->comboConfigurations->setEnabled(false);

    m_ui->btnConnect->setText(tr("&Disconnect"));

    QWidget::hide();
}

// EOF <stefan@scheler.com>
