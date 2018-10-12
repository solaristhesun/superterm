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

#ifndef PORTITEMDELEGATE_H
#define PORTITEMDELEGATE_H

#include <QItemDelegate>

class PortItemView;

class PortItemDelegate : public QItemDelegate
{
public:
    PortItemDelegate();

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    PortItemView* view_;
};

#endif // PORTITEMDELEGATE_H
