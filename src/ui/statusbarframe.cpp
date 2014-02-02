#include <QTimer>

#include "statusbarframe.h"
#include "ui_statusbarframe.h"

StatusBarFrame::StatusBarFrame(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::StatusBarFrame)
{
    m_ui->setupUi(this);

    hide();
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
        setStyleProperty("error", true);
        m_ui->btnClose->show();
    }
    else
    {
        setStyleProperty("error", false);
        m_ui->btnClose->hide();
        QTimer::singleShot(3000, this, SLOT(hide()));
    }

    show();
}

void StatusBarFrame::setStyleProperty(const char* name, const QVariant& variant)
{
    setProperty(name, variant);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

// EOF <stefan@scheler.com>
