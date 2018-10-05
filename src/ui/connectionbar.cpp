#include <QDir>
#include <QDebug>
#include <QSerialPort>

#include "session/session.h"
#include "misc/globals.h"
#include "ui/connectionbar.h"
#include "ui_connectionbar.h"

ConnectionBar::ConnectionBar(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::ConnectionBar)
{
    ui_->setupUi(this);

    fillComboBoxes();

    ui_->lineEditBaudRate->setText("9600");
    ui_->lineEditBaudRate->hide();
    ui_->comboDataBits->hide();
    ui_->comboFlowControl->hide();
    ui_->comboParity->hide();
    ui_->comboStopBits->hide();
    ui_->comboConfigurations->hide();
    ui_->btnSave->hide(); // save feature for now

    connect(ui_->btnConnect, &QPushButton::clicked, this, &ConnectionBar::connectClicked);
    connect(ui_->btnSave, &QPushButton::clicked, this, &ConnectionBar::saveClicked);
    connect(ui_->comboConfigurations, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated), this, &ConnectionBar::configurationChanged);
}

ConnectionBar::~ConnectionBar()
{
    delete ui_;
}

void ConnectionBar::showEvent(QShowEvent* event)
{
    QDir        dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList() << "*.xml", QDir::Files);
    ui_->comboConfigurations->clear();
    ui_->comboConfigurations->insertItems(0, files);

    if (!files.isEmpty())
    {
        ui_->comboConfigurations->insertItem(0, tr("Select configuration"));
        ui_->comboConfigurations->setCurrentIndex(0);
        ui_->comboConfigurations->show();
    }

    QWidget::showEvent(event);
}

void ConnectionBar::setPortEnumerator(PortEnumerator* pe)
{
    ui_->comboPorts->setPortEnumerator(pe);
}

QString ConnectionBar::getDeviceName() const
{
    return ui_->comboPorts->currentPortInfo().portName();
}

void ConnectionBar::setDeviceName(const QString& deviceName)
{
    ui_->comboPorts->setCurrentDeviceName(deviceName);
}

QString ConnectionBar::getDeviceDesc() const
{
    return ui_->comboPorts->currentPortInfo().description();
}

QString ConnectionBar::getBaudRate() const
{
    if (ui_->lineEditBaudRate->isVisible())
    {
        return ui_->lineEditBaudRate->text();
    }
    else
    {
        return ui_->comboBaudRates->currentText();
    }
}

void ConnectionBar::setBaudRate(const QString& baudRate)
{
    if (ui_->lineEditBaudRate->isVisible())
    {
        ui_->comboBaudRates->setCurrentText("custom");
        ui_->lineEditBaudRate->setText(baudRate);
    }
    else
    {
        ui_->comboBaudRates->setCurrentText(baudRate);
    }
}

QString ConnectionBar::getDataBits() const
{
    return ui_->comboDataBits->currentText();
}

void ConnectionBar::setDataBits(const QString& dataBits)
{
    ui_->comboDataBits->setCurrentText(dataBits);
}

QString ConnectionBar::getParity() const
{
    return ui_->comboParity->currentText();
}

void ConnectionBar::setParity(const QString& parity)
{
    ui_->comboParity->setCurrentText(parity);
}

QString ConnectionBar::getStopBits() const
{
    return ui_->comboStopBits->currentText();
}

void ConnectionBar::setStopBits(const QString& stopBits)
{
    ui_->comboStopBits->setCurrentText(stopBits);
}

QString ConnectionBar::getFlowControl() const
{
    return ui_->comboFlowControl->currentText();
}

void ConnectionBar::setFlowControl(const QString& flowControl)
{
    ui_->comboFlowControl->setCurrentText(flowControl);
}

void ConnectionBar::loadFromSession(Session* session)
{
    qDebug() << *session << session->getDeviceDesc();
    ui_->comboPorts->setPort(session->getDeviceName());

    if (ui_->comboBaudRates->findText(QString::number(session->getBaudRate())) >= 0)
    {
        ui_->comboBaudRates->setCurrentText(QString::number(session->getBaudRate()));
    }
    else
    {
        ui_->comboBaudRates->setCurrentText("custom");
        ui_->lineEditBaudRate->setText(QString::number(session->getBaudRate()));
    }

    ui_->comboDataBits->setCurrentText(QString::number(session->getDataBits()));

    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(session->getParity());
    ui_->comboParity->setCurrentText(Globals::ParityNameMap.value(parity));

    QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(session->getStopBits());
    ui_->comboStopBits->setCurrentText(Globals::StopBitsNameMap.value(stopBits));

    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(session->getFlowControl());
    ui_->comboFlowControl->setCurrentText(Globals::FlowControlNameMap.value(flowControl));

    ui_->btnConnect->setEnabled(true); // FIXME: unsauber
}

void ConnectionBar::fillComboBoxes()
{
    QComboBox* combo = nullptr;

    // fill baud rates
    combo = ui_->comboBaudRates;

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
    combo = ui_->comboDataBits;
    combo->clear();
    combo->addItem("5", qVariantFromValue(QSerialPort::Data5));
    combo->addItem("6", qVariantFromValue(QSerialPort::Data6));
    combo->addItem("7", qVariantFromValue(QSerialPort::Data7));
    combo->addItem("8", qVariantFromValue(QSerialPort::Data8));
    combo->setCurrentIndex(3);

    // fill parity
    combo = ui_->comboParity;
    combo->clear();
    combo->addItem(tr("None"), qVariantFromValue(QSerialPort::NoParity));
    combo->addItem(tr("Even"), qVariantFromValue(QSerialPort::EvenParity));
    combo->addItem(tr("Odd"), qVariantFromValue(QSerialPort::OddParity));
    combo->addItem(tr("Space"), qVariantFromValue(QSerialPort::SpaceParity));
    combo->addItem(tr("Mark"), qVariantFromValue(QSerialPort::MarkParity));

    // fill stop bits
    combo = ui_->comboStopBits;
    combo->clear();
    combo->addItem("1", qVariantFromValue(QSerialPort::OneStop));
    combo->addItem("1.5", qVariantFromValue(QSerialPort::OneAndHalfStop));
    combo->addItem("2", qVariantFromValue(QSerialPort::TwoStop));
    combo->setCurrentIndex(0);

    // fill flow control
    combo = ui_->comboFlowControl;
    combo->clear();
    combo->addItem(tr("None"), qVariantFromValue(QSerialPort::NoFlowControl));
    combo->addItem(tr("Xon/Xoff"), qVariantFromValue(QSerialPort::SoftwareControl));
    combo->addItem(tr("Hardware"), qVariantFromValue(QSerialPort::HardwareControl));
    combo->setCurrentIndex(0);
}

void ConnectionBar::onMoreClicked()
{
    if (!ui_->comboParity->isVisible())
    {
        ui_->comboDataBits->show();
        ui_->comboFlowControl->show();
        ui_->comboParity->show();
        ui_->comboStopBits->show();
        ui_->btnMore->setText(tr("<< &Less"));
    }
    else
    {
        ui_->comboDataBits->hide();
        ui_->comboFlowControl->hide();
        ui_->comboParity->hide();
        ui_->comboStopBits->hide();
        ui_->btnMore->setText(tr("&More >>"));
    }
}

void ConnectionBar::onComboChanged()
{
    qDebug() << "[slot] onComboChanged";

    if (ui_->comboPorts->currentIndex() != 0 && ui_->comboBaudRates->currentIndex() != 0)
    {
        ui_->btnConnect->setEnabled(true);
        ui_->btnSave->setEnabled(true);
    }
    else
    {
        ui_->btnConnect->setEnabled(false);
        ui_->btnSave->setEnabled(false);
    }

    if (ui_->comboBaudRates->currentText() == "custom")
    {
        ui_->lineEditBaudRate->show();
    }
    else
    {
        ui_->lineEditBaudRate->hide();
    }
}

void ConnectionBar::onDisconnected()
{
    ui_->comboPorts->setEnabled(true);
    ui_->lineEditBaudRate->setEnabled(true);
    ui_->comboBaudRates->setEnabled(true);
    ui_->comboDataBits->setEnabled(true);
    ui_->comboFlowControl->setEnabled(true);
    ui_->comboParity->setEnabled(true);
    ui_->comboStopBits->setEnabled(true);
    ui_->comboConfigurations->setEnabled(true);

    ui_->btnConnect->setText(tr("&Connect"));

    QWidget::show();
}

void ConnectionBar::onConnected()
{
    ui_->comboPorts->setEnabled(false);
    ui_->lineEditBaudRate->setEnabled(false);
    ui_->comboBaudRates->setEnabled(false);
    ui_->comboDataBits->setEnabled(false);
    ui_->comboFlowControl->setEnabled(false);
    ui_->comboParity->setEnabled(false);
    ui_->comboStopBits->setEnabled(false);
    ui_->comboConfigurations->setEnabled(false);

    ui_->btnConnect->setText(tr("&Disconnect"));

    QWidget::hide();
}

// EOF <stefan@scheler.com>
