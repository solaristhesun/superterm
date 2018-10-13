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

#ifndef CONSOLELINEITEMDELEGATE_H
#define CONSOLELINEITEMDELEGATE_H

#include <QItemDelegate>

class ConsoleView;

class ConsoleLineItemDelegate : public QItemDelegate
{
public:
    explicit ConsoleLineItemDelegate(ConsoleView* parent);

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    int timestampWidth() const;

    void updateFontMetrics();

private:
    int charsPerLine(const int width) const;

private:
    ConsoleView* consoleView_;
    QString      timestampFormat_;
    int          timestampWidth_;
    int          cursorWidth_;
    int          fontHeight_;
    int          fontDescent_;
    int          charsPerLine_;
};

#endif // CONSOLELINEITEMDELEGATE_H
