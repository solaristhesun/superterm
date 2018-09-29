#include <QPainter>

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
    QFontMetrics metrics(consoleView_->font());
    return QSize(1,metrics.height()+4);
}

void ConsoleLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics metrics(consoleView_->font());

    const int cursorWidth = metrics.width(" ");

    QRect adjusted = option.rect.adjusted(2,+2,-2,-2);

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
        painter->drawRect(option.rect.adjusted(0,0,0,-2));
    }

     painter->setFont(consoleView_->font());
     painter->setPen(QColor(Qt::white).darker(150));

    if (consoleView_->timestampsEnabled())
    {
        painter->setPen(QColor(Qt::white).darker(150));
        painter->drawText(adjusted, Qt::AlignLeft, timestamp);
        xTextStart += getTimestampWidth() + 15;
    }

    adjusted.setLeft(xTextStart);

    // draw text
    painter->setPen(consoleView_->textColor());
    painter->drawText(adjusted, Qt::AlignLeft, text);

    // draw cursor
    if (index.row() == index.model()->rowCount()-1)
    {
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->drawRect(xTextStart + metrics.width(text) + 1, option.rect.y(), cursorWidth, option.rect.height()-4);
    }

    painter->restore();
}

int ConsoleLineItemDelegate::getTimestampWidth() const
{
    QFontMetrics metrics(consoleView_->font());
    return metrics.width(timestampFormat_);
}

// EOF <stefan@scheler.com>
