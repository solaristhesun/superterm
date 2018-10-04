#include "portitemview.h"
#include "ui_portitemview.h"

PortItemView::PortItemView(QWidget *parent)
    : QFrame(parent)
    , ui_(new Ui::PortItemView)
{
    ui_->setupUi(this);
}

PortItemView::~PortItemView()
{
    delete ui_;
}

void PortItemView::setDescription(const QString& description)
{
    ui_->descriptionLabel->setText(description);
}

void PortItemView::setPortName(const QString& portName)
{
    ui_->portNameLabel->setText(portName);
}

void PortItemView::setBusyFlag(const bool bBusy)
{
    if (bBusy)
    {
        ui_->busyLabel->setStyleSheet("background-color: red");
    }
    else
    {
        ui_->busyLabel->setStyleSheet("background-color: green");
    }
}

// EOF <stefan@scheler.com>
