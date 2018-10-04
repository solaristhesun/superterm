#include "renametabframe.h"
#include "ui_renametabframe.h"

RenameTabFrame::RenameTabFrame(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::RenameTabFrame)
{
    ui_->setupUi(this);
    QFrame::hide();
}

RenameTabFrame::~RenameTabFrame()
{
    delete ui_;
}

void RenameTabFrame::setTabLabel(const QString& text)
{
    ui_->labelEdit->setText(text);
}

QString RenameTabFrame::tabLabel() const
{
    return ui_->labelEdit->text();
}

void RenameTabFrame::show()
{
    QFrame::show();
    ui_->labelEdit->selectAll();
    ui_->labelEdit->setFocus();
}

void RenameTabFrame::onApplyClicked()
{
    emit applyPressed();
}

// EOF <stefan@scheler.com>
