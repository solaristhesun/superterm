#include <QFileDialog>
#include <QFileInfo>

#include "ui_logfilepanel.h"
#include "ui/logfilepanel.h"

LogfilePanel::LogfilePanel(QWidget *parent)
    : QFrame(parent)
    , m_ui(new Ui::LogfilePanel)
    , m_bEnabled(false)
{
    m_ui->setupUi(this);

    hide();
}

LogfilePanel::~LogfilePanel()
{
    delete m_ui;
}

void LogfilePanel::showFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::homePath(),
                               tr("Logfiles (*.log)"));

    if (!fileName.isEmpty())
    {
        m_ui->editFileName->setText(fileName);
    }
}

void LogfilePanel::onFileNameChanged(const QString & text)
{
    QFileInfo fileInfo(text);

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
        m_ui->btnStart->setText(tr("&Stop logging"));
        hide();
    }
    else
    {
        emit loggingStopped();
        m_ui->btnSelect->setEnabled(true);
        m_ui->editFileName->setEnabled(true);
        m_ui->btnStart->setText(tr("&Start logging"));
    }
}

QString LogfilePanel::getLogFileName(void) const
{
    return m_ui->editFileName->text();
}

// EOF <stefan@scheler.com>
