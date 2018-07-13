#include "renametabframe.h"
#include "ui_renametabframe.h"

RenameTabFrame::RenameTabFrame(QWidget* parent)
    : QFrame(parent),
    ui(new Ui::RenameTabFrame)
{
    ui->setupUi(this);
    hide();
}

RenameTabFrame::~RenameTabFrame()
{
    delete ui;
}

void RenameTabFrame::setText(const QString& text)
{
    ui->labelEdit->setText(text);
}

QString RenameTabFrame::getText() const
{
    return ui->labelEdit->text();
}

void RenameTabFrame::show()
{
    QFrame::show();
    ui->labelEdit->selectAll();
    ui->labelEdit->setFocus();
}

void RenameTabFrame::onApplyClicked()
{
    emit applyPressed();
}
