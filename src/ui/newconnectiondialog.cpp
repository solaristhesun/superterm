#include "newconnectiondialog.h"
#include "ui_newconnectiondialog.h"

NewConnectionDialog::NewConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConnectionDialog)
{
    ui->setupUi(this);
}

NewConnectionDialog::~NewConnectionDialog()
{
    delete ui;
}
