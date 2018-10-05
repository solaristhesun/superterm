#include <QPainter>
#include <QElapsedTimer>
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
    , charsPerLine_(0)
{
    // currently empty
}

QSize ConsoleLineItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int numChars = index.data(Qt::SizeHintRole).toInt();
    const int numLines = (charsPerLine_ > 0) ? (numChars / charsPerLine_) + 1 : 1;

    return QSize(option.rect.width(), (fontHeight_+2) * numLines);
}

void ConsoleLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QElapsedTimer timer; timer.start();

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
        painter->setPen(consoleView_->textColor().darker(150));
        painter->drawText(textRect, timestamp);
        painter->drawLine(timestampWidth()+2,option.rect.y(),timestampWidth()+2,option.rect.bottom());
        xTextStart += timestampWidth() + 7;
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
        QString lineText = text.mid(i*numChars, numChars);
        painter->drawText(textRect.x(), option.rect.y()+ (fontHeight_+1) + (i)*(fontHeight_+2)-metrics.descent(),lineText);

        if (i+1 == numLines)
        {
            lastLineWidth = metrics.width(lineText);
        }
    }

    // draw cursor
    if (index.row() == index.model()->rowCount()-1)
    {
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->drawRect(xTextStart + lastLineWidth, option.rect.y() + (numLines-1)*fontHeight_, cursorWidth_, fontHeight_);
    }

    qDebug() << "Paint" << index.row() << option.rect << timer.nsecsElapsed();
}

int ConsoleLineItemDelegate::timestampWidth() const
{
    return timestampWidth_;
}

int ConsoleLineItemDelegate::charsPerLine(int width) const
{
    int pixelsAvailable = width;

    if (consoleView_->timestampsEnabled())
    {
        pixelsAvailable -= timestampWidth() + 7;
    }

    return pixelsAvailable / cursorWidth_;
}

void ConsoleLineItemDelegate::updateFontMetrics()
{
    QFontMetrics metrics(consoleView_->font());
    timestampWidth_ = metrics.width(timestampFormat_);
    cursorWidth_ = metrics.averageCharWidth();
    fontHeight_ = metrics.height();
    charsPerLine_ = charsPerLine(consoleView_->width() - consoleView_->verticalScrollBar()->width());
}

// EOF <stefan@scheler.com>
