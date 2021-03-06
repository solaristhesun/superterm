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

#include <QPainter>
#include <QScrollBar>
#include <QDebug>

#include "consolelineitemdelegate.h"
#include "models/consoleline.h"
#include "ui/consoleview/consoleview.h"

ConsoleLineItemDelegate::ConsoleLineItemDelegate(ConsoleView *consoleView)
    : QItemDelegate(consoleView)
    , consoleView_(consoleView)
    , timestampFormat_("[yyyy-MM-dd HH:mm:ss.zzz]")
    , timestampWidth_(0)
    , cursorWidth_(0)
    , fontHeight_(0)
    , fontDescent_(0)
    , charsPerLine_(0)
{
    // currently empty
}

QSize ConsoleLineItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int numChars = index.data(Qt::SizeHintRole).toInt();
    const int numLines = (charsPerLine_ > 0) ? (numChars / charsPerLine_) + 1 : 1;

    const QSize size(option.rect.width(), (fontHeight_+2) * numLines);

    return size;
}

void ConsoleLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ConsoleLine line = index.data().value<ConsoleLine>();
    QString timestamp = line.timestamp().toString(timestampFormat_);
    QString text = line.text();
    QColor backgroundColor = line.color();

    int xTextStart = 0;

    QRect textRect = option.rect.adjusted(0,+1,0,-1);

    QFontMetrics metrics(consoleView_->font());
    painter->setFont(consoleView_->font());

    // paint background of line
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, consoleView_->palette().highlight());
    }
    else if (backgroundColor.isValid())
    {
        painter->fillRect(option.rect, backgroundColor);
    }

    // paint timestamp
    if (consoleView_->timestampsEnabled())
    {
        painter->setPen(consoleView_->textColor().darker(105));
        painter->drawText(textRect, timestamp);
        painter->drawLine(timestampWidth()+2,option.rect.y(),timestampWidth()+2,option.rect.bottom());
        xTextStart += timestampWidth() + 6;
    }

    painter->setPen(QColor(Qt::white).darker(150));

    textRect.setLeft(xTextStart);

    // draw text
    painter->setPen(consoleView_->textColor());

    const int numChars = text.length();
    const int numLines = (charsPerLine_ > 0) ? (numChars / charsPerLine_) + 1 : 1;

    int lastLineWidth = 0;
    for (int i = 0; i < numLines; i++)
    {
        const QString lineText = text.mid(i*charsPerLine_, charsPerLine_);

        painter->drawText(textRect.x(), option.rect.y()+(fontHeight_+1)+(i)*(fontHeight_+2)-fontDescent_, lineText);

        if (i+1 == numLines)
        {
            lastLineWidth = metrics.width(lineText);
        }
    }

    // draw cursor
    if (index.row() == index.model()->rowCount()-1)
    {
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->drawRect(xTextStart + lastLineWidth, option.rect.y() + (numLines-1)*(fontHeight_+2), cursorWidth_, fontHeight_);
    }
}

int ConsoleLineItemDelegate::timestampWidth() const
{
    return timestampWidth_;
}

int ConsoleLineItemDelegate::charsPerLine(const int width) const
{
    int pixelsAvailable = width;

    if (consoleView_->timestampsEnabled())
    {
        pixelsAvailable -= timestampWidth() + 6;
    }

    return pixelsAvailable / cursorWidth_;
}

void ConsoleLineItemDelegate::updateFontMetrics()
{
    QFontMetrics metrics(consoleView_->font());
    timestampWidth_ = metrics.width(timestampFormat_);
    cursorWidth_ = metrics.averageCharWidth();
    fontHeight_ = metrics.height();
    fontDescent_ = metrics.descent();
    charsPerLine_ = charsPerLine(consoleView_->width() - consoleView_->verticalScrollBar()->width());
}

// EOF <stefan@scheler.com>
