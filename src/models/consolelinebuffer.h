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

#ifndef CONSOLELINEBUFFER_H
#define CONSOLELINEBUFFER_H

#include <QAbstractListModel>
#include <QPair>
#include <QStringList>

#include "models/consoleline.h"

class QFile;
class HighlightingsModel;

class ConsoleLineBuffer : public QAbstractListModel
{
    Q_OBJECT
public:
    ConsoleLineBuffer(HighlightingsModel* highlightingsModel);
    virtual ~ConsoleLineBuffer() override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool startLogging(QString fileName);
    void stopLogging();
    bool isLogging() const;

    void append(QByteArray data);
    void append(QString data);
    void clear();

private slots:
    void refreshHighlighting();
    void completeInsertion();

private:
    void appendToLastLine(QChar c);
    void createNewLine();
    void refreshSingleHighlighting(ConsoleLine& line);
    void writeLineToLogFile(ConsoleLine& line);

private:
    QVector<ConsoleLine> list_;
    HighlightingsModel*  highlightingsModel_;
    QFile*               logFile_;
    bool                 bInsertStarted_;
    int                  firstRow_;
    int                  lastRow_;
};

#endif // CONSOLELINEBUFFER_H
