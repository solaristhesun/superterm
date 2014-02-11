#include <QFileDialog>

#include "logfilepanel.h"
#include "ui_logfilepanel.h"

LogfilePanel::LogfilePanel(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::LogfilePanel),
    m_bEnabled(false)
{
    m_ui->setupUi(this);
}

LogfilePanel::~LogfilePanel()
{
    delete m_ui;
}

void LogfilePanel::showFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::homePath(),
                               tr("Images (*.png *.xpm *.jpg)"));

    if (!fileName.isEmpty())
    {
        m_ui->editFileName->setText(fileName);
    }
}

void LogfilePanel::onFileNameChanged(const QString & text)
{
    m_ui->btnStart->setEnabled(!text.isEmpty());
}

void LogfilePanel::onStartButtonClicked(void)
{
    m_bEnabled = !m_bEnabled;

    if (m_bEnabled)
    {
        emit loggingStarted();
        m_ui->btnSelect->setEnabled(false);
        m_ui->editFileName->setEnabled(false);
        m_ui->btnStart->setText(tr("Stop logging"));
        hide();
    }
    else
    {
        emit loggingStopped();
        m_ui->btnSelect->setEnabled(true);
        m_ui->editFileName->setEnabled(true);
        m_ui->btnStart->setText(tr("Start logging"));
    }
}

QString LogfilePanel::getLogFileName(void) const
{
    return m_ui->editFileName->text();
}

// EOF <stefan.scheler@bbraun.com>
