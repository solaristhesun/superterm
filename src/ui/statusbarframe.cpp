#include <QTimer>

#include "ui_statusbarframe.h"
#include "ui/statusbarframe.h"

CStatusBarFrame::CStatusBarFrame(QWidget* parent)
    : QFrame(parent)
    , m_ui(new Ui::CStatusBarFrame)
{
    m_ui->setupUi(this);

    setProperty("theme", "default");
    setupWaitingSpinner();

    hide();

    connect(m_ui->btnCancel, &QPushButton::pressed, this, &CStatusBarFrame::cancelReconnection);
}

CStatusBarFrame::~CStatusBarFrame()
{
    delete m_ui;
}

void CStatusBarFrame::showMessage(const QString& message, const int timeout)
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
        QTimer::singleShot(timeout, this, &CStatusBarFrame::hideOnTimeout);
    }

    show();
}

void CStatusBarFrame::hideOnTimeout()
{
    if (QObject::property("theme") == "default")
    {
        QWidget::hide();
    }
}

void CStatusBarFrame::showProgressMessage(const QString& message)
{
    m_ui->labelMessage->setText(message);

    setStyleProperty("theme", "warning");
    m_ui->btnClose->hide();
    m_ui->btnCancel->show();
    m_ui->waitingSpinner->start();

    show();
}

void CStatusBarFrame::hideProgressMessage()
{
    m_ui->waitingSpinner->stop();
    hide();
}

void CStatusBarFrame::setStyleProperty(const char* name, const QVariant& variant)
{
    setProperty(name, variant);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void CStatusBarFrame::setupWaitingSpinner()
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
