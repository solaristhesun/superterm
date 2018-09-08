#include "portitemview.h"
#include "ui_portitemview.h"

PortItemView::PortItemView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PortItemView)
{
    ui->setupUi(this);
}

PortItemView::~PortItemView()
{
    delete ui;
}

void PortItemView::setDescription(QString description)
{
    ui->descriptionLabel->setText(description);
}

void PortItemView::setPortName(QString portName)
{
    ui->portNameLabel->setText(portName);
}
