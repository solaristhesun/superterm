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

#ifndef LOGFILEPANEL_H
#define LOGFILEPANEL_H

#include <QFrame>

namespace Ui
{
class LogfilePanel;
}

class LogfilePanel : public QFrame
{
    Q_OBJECT

public:
    explicit LogfilePanel(QWidget* parent = nullptr);
    ~LogfilePanel() override;

    QString getLogFileName() const;
    void    setLogFileName(QString fileName);

public slots:
    void showFileDialog();
    void onFileNameChanged(const QString& text);
    void onStartButtonClicked();

signals:
    void loggingStarted();
    void loggingStopped();

private:
    Ui::LogfilePanel* ui_;
    bool              bEnabled_;
};

#endif // LOGFILEPANEL_H
