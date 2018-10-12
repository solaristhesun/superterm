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

#ifndef HIGHLIGHTINGSMODEL_H
#define HIGHLIGHTINGSMODEL_H

#include <QAbstractListModel>

#include "models/highlighting.h"

class HighlightingsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HighlightingsModel(QObject* parent = nullptr);

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void                setHighlightings(QList<Highlighting> highlightings);
    void                addHighlighting(Highlighting highlighting);
    QList<Highlighting> highlightings() const;
    void                remove(const QModelIndex& index);
    void                removeAll();
    QColor              lineColor(QString line);

signals:
    void highlightingChanged();

private:
    QList<Highlighting> highlightings_;
};

#endif // HIGHLIGHTINGSMODEL_H
