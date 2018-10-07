#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

#include "ui_logfilepanel.h"
#include "ui/logfilepanel.h"

LogfilePanel::LogfilePanel(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::LogfilePanel)
    , bEnabled_(false)
{
    ui_->setupUi(this);

    QFrame::hide();
}

LogfilePanel::~LogfilePanel()
{
    delete ui_;
}

void LogfilePanel::showFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    getLogFileName(),
                                                    tr("Logfiles (*.log)"));

    if (!fileName.isEmpty())
    {
        ui_->editFileName->setText(fileName);
    }
}

void LogfilePanel::onFileNameChanged(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    QFileInfo dirInfo(fileInfo.absolutePath());

    const bool bButtonEnabled = fileInfo.absoluteDir().exists()
        && dirInfo.isWritable()
        && !fileInfo.fileName().isEmpty()
        && !fileInfo.isDir();

    ui_->btnStart->setEnabled(bButtonEnabled);
}

void LogfilePanel::onStartButtonClicked()
{
    bEnabled_ = !bEnabled_;

    if (bEnabled_)
    {
        emit loggingStarted();
        ui_->btnSelect->setEnabled(false);
        ui_->editFileName->setEnabled(false);
        ui_->btnStart->setText(tr("&Stop logging"));
        QFrame::hide();
    }
    else
    {
        emit loggingStopped();
        ui_->btnSelect->setEnabled(true);
        ui_->editFileName->setEnabled(true);
        ui_->btnStart->setText(tr("&Start logging"));
    }
}

QString LogfilePanel::getLogFileName() const
{
    return ui_->editFileName->text();
}

void LogfilePanel::setLogFileName(QString fileName)
{
    ui_->editFileName->setText(fileName);
}

// EOF <stefan@scheler.com>
