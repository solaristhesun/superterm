/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

#include "ui_logfilepanel.h"
#include "ui/logfilepanel.h"

LogfilePanel::LogfilePanel(QWidget* parent)
    : QFrame(parent)
    , ui_(new Ui::LogfilePanel)
    , bEnabled_(false)
    , filename_()
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
        setLogFileName(fileName);
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
    return filename_;
}

void LogfilePanel::setLogFileName(QString fileName)
{
    filename_ = fileName;
    ui_->editFileName->setText(QDir::toNativeSeparators(fileName));
}

// EOF <stefan@scheler.com>
