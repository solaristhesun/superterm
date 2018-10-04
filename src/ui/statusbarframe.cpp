#include <QTimer>
#include <QStyle>

#include "ui_statusbarframe.h"
#include "ui/statusbarframe.h"

StatusBarFrame::StatusBarFrame(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::StatusBarFrame)
{
    ui_->setupUi(this);

    QFrame::hide();
    QFrame::setProperty("theme", "default");

    setupWaitingSpinner();

    connect(ui_->btnCancel, &QPushButton::pressed, this, &StatusBarFrame::cancelReconnection);
}

StatusBarFrame::~StatusBarFrame()
{
    delete ui_;
}

void StatusBarFrame::showMessage(const QString& message, const int timeout)
{
    ui_->labelMessage->setText(message);

    if (timeout == 0)
    {
        setStyleProperty("theme", "error");
        ui_->waitingSpinner->stop();
        ui_->btnCancel->hide();
        ui_->btnClose->show();
    }
    else
    {
        setStyleProperty("theme", "default");
        ui_->waitingSpinner->stop();
        ui_->btnCancel->hide();
        ui_->btnClose->hide();
        QTimer::singleShot(timeout, this, &StatusBarFrame::hideOnTimeout);
    }

    QFrame::show();
}

void StatusBarFrame::hideOnTimeout()
{
    if (QObject::property("theme") == "default")
    {
        QWidget::hide();
    }
}

void StatusBarFrame::showProgressMessage(const QString& message)
{
    ui_->labelMessage->setText(message);

    setStyleProperty("theme", "warning");
    ui_->btnClose->hide();
    ui_->btnCancel->show();
    ui_->waitingSpinner->start();

    QFrame::show();
}

void StatusBarFrame::hideProgressMessage()
{
    ui_->waitingSpinner->stop();
    hide();
}

void StatusBarFrame::setStyleProperty(const char* name, const QVariant& variant)
{
    QFrame::setProperty(name, variant);
    QFrame::style()->unpolish(this);
    QFrame::style()->polish(this);
    QFrame::update();
}

void StatusBarFrame::setupWaitingSpinner()
{
    ui_->waitingSpinner->setRoundness(70.0);
    ui_->waitingSpinner->setMinimumTrailOpacity(15.0);
    ui_->waitingSpinner->setTrailFadePercentage(70.0);
    ui_->waitingSpinner->setNumberOfLines(8);
    ui_->waitingSpinner->setLineLength(5);
    ui_->waitingSpinner->setLineWidth(5);
    ui_->waitingSpinner->setInnerRadius(5);
}

// EOF <stefan@scheler.com>
