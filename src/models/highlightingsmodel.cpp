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

#include <QIcon>
#include <QDebug>

#include "highlightingsmodel.h"

HighlightingsModel::HighlightingsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    // currently empty
}


int HighlightingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return highlightings_.count();
}

QVariant HighlightingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= highlightings_.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
        return QVariant::fromValue(highlightings_.at(index.row()).pattern);

    if (role == Qt::DecorationRole)
    {
        QPixmap pixmap(10, 10);
        pixmap.fill(highlightings_.at(index.row()).color);

        return QVariant::fromValue(QIcon(pixmap));
    }

    return QVariant();
}

void HighlightingsModel::addHighlighting(Highlighting highlighting)
{
    beginInsertRows(QModelIndex(), highlightings_.count(), highlightings_.count());
    highlightings_.append(highlighting);
    endInsertRows();

    emit highlightingChanged();
}

void HighlightingsModel::remove(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    highlightings_.removeAt(index.row());
    endRemoveRows();

    emit highlightingChanged();
}

void HighlightingsModel::removeAll()
{
    beginResetModel();
    highlightings_.clear();
    endResetModel();

    emit highlightingChanged();
}

void HighlightingsModel::setHighlightings(QList<Highlighting> highlightings)
{
    beginResetModel();
    highlightings_ = highlightings;
    endResetModel();

    emit highlightingChanged();
}

QList<Highlighting> HighlightingsModel::highlightings() const
{
    return highlightings_;
}

QColor HighlightingsModel::lineColor(QString line)
{
    QColor color;

    for (int i = 0; i < highlightings_.size(); i++)
    {
        Highlighting highlighting = highlightings_.at(i);

        if (line.contains(highlighting.pattern))
        {
            return highlighting.color;
        }
    }

    return color;
}

// EOF <stefan@scheler.com>
