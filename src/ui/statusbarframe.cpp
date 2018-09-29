#include <QTimer>
#include <QStyle>

#include "ui_statusbarframe.h"
#include "ui/statusbarframe.h"

StatusBarFrame::StatusBarFrame(QWidget* parent)
    : QFrame(parent)
    , m_ui(new Ui::StatusBarFrame)
{
    m_ui->setupUi(this);

    setProperty("theme", "default");
    setupWaitingSpinner();

    hide();

    connect(m_ui->btnCancel, &QPushButton::pressed, this, &StatusBarFrame::cancelReconnection);
}

StatusBarFrame::~StatusBarFrame()
{
    delete m_ui;
}

void StatusBarFrame::showMessage(const QString& message, const int timeout)
{
    m_ui->labelMessage->setText(message);

    if (timeout == 0)
    {
        setStyleProperty("theme", "error");
        m_ui->waitingSpinner->stop();
        m_ui->btnCancel->hide();
        m_ui->btnClose->show();
    }
    else
    {
        setStyleProperty("theme", "default");
        m_ui->waitingSpinner->stop();
        m_ui->btnCancel->hide();
        m_ui->btnClose->hide();
        QTimer::singleShot(timeout, this, &StatusBarFrame::hideOnTimeout);
    }

    show();
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
    m_ui->labelMessage->setText(message);

    setStyleProperty("theme", "warning");
    m_ui->btnClose->hide();
    m_ui->btnCancel->show();
    m_ui->waitingSpinner->start();

    show();
}

void StatusBarFrame::hideProgressMessage()
{
    m_ui->waitingSpinner->stop();
    hide();
}

void StatusBarFrame::setStyleProperty(const char* name, const QVariant& variant)
{
    setProperty(name, variant);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void StatusBarFrame::setupWaitingSpinner()
{
    m_ui->waitingSpinner->setRoundness(70.0);
    m_ui->waitingSpinner->setMinimumTrailOpacity(15.0);
    m_ui->waitingSpinner->setTrailFadePercentage(70.0);
    m_ui->waitingSpinner->setNumberOfLines(8);
    m_ui->waitingSpinner->setLineLength(5);
    m_ui->waitingSpinner->setLineWidth(5);
    m_ui->waitingSpinner->setInnerRadius(5);
}

// EOF <stefan@scheler.com>
