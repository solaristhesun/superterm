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

#ifndef HIGHLIGHTINGSFRAME_H
#define HIGHLIGHTINGSFRAME_H

#include <QColor>
#include <QFrame>
#include <QItemSelection>

#include "models/highlighting.h"

namespace Ui
{
class HighlightingsFrame;
}

class HighlightingsModel;
class QSortFilterProxyModel;

class HighlightingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HighlightingsFrame(QWidget* parent = nullptr);
    virtual ~HighlightingsFrame() override;

    void showEvent(QShowEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void setModel(HighlightingsModel* model);

public slots:
    void addHighlighting();
    void deleteHighlighting();
    void onTextEdited(const QString& text);
    void onSelectionChanged();
    void changeColor();
    void deleteAll();
    void selectionChanged(const QItemSelection&, const QItemSelection&);

private:
    Ui::HighlightingsFrame* ui_;
    Highlighting            highlighting_;
    QSortFilterProxyModel*  proxyModel_;
    HighlightingsModel*     model_;

    void refreshColorButton();
};

#endif // HIGHLIGHTINGSFRAME_H
