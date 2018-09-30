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
{
    // currently empty
}

QSize ConsoleLineItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ConsoleLine line = qvariant_cast<ConsoleLine>(index.data());
    QFontMetrics metrics(consoleView_->font());

    QSize size;

    int numChars = charsPerLine(consoleView_->width() - consoleView_->verticalScrollBar()->width());
    int numLines = 1;
    if (numChars > 0)
        numLines = line.text().length() / numChars + 1;

    size.setHeight((metrics.height()+2)*numLines);
    size.setWidth(option.rect.width());

    return size;
}

void ConsoleLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics metrics(consoleView_->font());

    const int cursorWidth = metrics.width(" ");

    QRect adjusted = option.rect.adjusted(0,+1,0,-1);

    ConsoleLine line = qvariant_cast<ConsoleLine>(index.data());
    QColor backgroundColor = line.color();
    painter->save();

    QString timestamp = line.timestamp().toString(timestampFormat_);
    QString text = line.text();

    int xTextStart = 0;

    if (backgroundColor.isValid())
    {
        painter->setPen(QColor(backgroundColor));
        painter->setBrush(QBrush(QColor(backgroundColor)));
        painter->drawRect(option.rect.adjusted(0,0,0,-1));
    }

     painter->setFont(consoleView_->font());
     painter->setPen(QColor(Qt::white).darker(150));

    if (consoleView_->timestampsEnabled())
    {
        painter->setPen(QColor(Qt::white).darker(150));
        painter->drawText(adjusted, Qt::AlignLeft, timestamp);
        painter->setPen(QColor("white").darker(150));
        painter->drawLine(getTimestampWidth()+5,0,getTimestampWidth()+5,option.rect.bottom());
        xTextStart += getTimestampWidth() + 11;
    }

    adjusted.setLeft(xTextStart);

    // draw text
    painter->setPen(consoleView_->textColor());

    int numChars = charsPerLine(consoleView_->width() - consoleView_->verticalScrollBar()->width());
    int numLines = 1;
    if (numChars > 0)
        numLines = line.text().length() / numChars + 1;

    int lastLineWidth = 0;
    for (int i = 0; i < numLines; i++)
    {
        QString lineText = text.mid(i*numChars, numChars);
        painter->drawText(adjusted.x(), option.rect.y()+ (metrics.height()+1) + (i)*(metrics.height()+2)-metrics.descent(),lineText);

        if (i+1 == numLines)
        {
            lastLineWidth = metrics.width(lineText);
        }
    }

    // draw cursor
    if (index.row() == index.model()->rowCount()-1)
    {
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->drawRect(xTextStart + lastLineWidth, option.rect.y() + (numLines-1)*metrics.height(), cursorWidth, metrics.height());
    }

    painter->restore();
}

int ConsoleLineItemDelegate::getTimestampWidth() const
{
    QFontMetrics metrics(consoleView_->font());
    return metrics.width(timestampFormat_);
}

int ConsoleLineItemDelegate::charsPerLine(int width) const
{
    QFontMetrics metrics(consoleView_->font());

    int pixelsAvailable = width;

    if (consoleView_->timestampsEnabled())
    {
        pixelsAvailable -= getTimestampWidth() + 11;
    }

    return pixelsAvailable / metrics.averageCharWidth();
}

// EOF <stefan@scheler.com>
