#include <QTimer>

#include "ui_statusbarframe.h"
#include "ui/statusbarframe.h"

CStatusBarFrame::CStatusBarFrame(QWidget* parent)
    : QFrame(parent)
    , m_ui(new Ui::CStatusBarFrame)
{
    m_ui->setupUi(this);

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
        setStyleProperty("error", true);
        m_ui->progressBar->hide();
        m_ui->btnCancel->hide();
        m_ui->btnClose->show();
    }
    else
    {
        setStyleProperty("error", false);
        m_ui->progressBar->hide();
        m_ui->btnCancel->hide();
        m_ui->btnClose->hide();
        QTimer::singleShot(timeout, this, SLOT(hide()));
    }

    show();
}

void CStatusBarFrame::showProgressMessage(const QString& message)
{
    m_ui->labelMessage->setText(message);

    setStyleProperty("error", false);
    m_ui->btnClose->hide();
    m_ui->btnCancel->show();
    m_ui->progressBar->show();

    show();
}

void CStatusBarFrame::hideProgressMessage()
{
    hide();
}

void CStatusBarFrame::setStyleProperty(const char* name, const QVariant& variant)
{
    setProperty(name, variant);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

// EOF <stefan@scheler.com>
