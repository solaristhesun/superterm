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
    QElapsedTimer timer; timer.start();
    // sizeHint is performance critical
    ConsoleLine line = qvariant_cast<ConsoleLine>(index.data());
quint64 t1 = timer.nsecsElapsed();
    QSize size;
    int numChars = charsPerLine_;
    int numLines = 1;
    if (numChars > 0)
        numLines = line.text().length() / numChars + 1;
quint64 t2 = timer.nsecsElapsed();
    size.setHeight((fontHeight_+2)*numLines);
    size.setWidth(option.rect.width());
    //qDebug() << "size" << index.row() <<  t1 << t2 << timer.nsecsElapsed();
    return size;
}

void ConsoleLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //qDebug() << "PAINT" << index.row();
    QFontMetrics metrics(consoleView_->font());

    QRect adjusted = option.rect.adjusted(0,+1,0,-1);

    ConsoleLine line = qvariant_cast<ConsoleLine>(index.data());
    QColor backgroundColor = line.color();
    painter->save();

    QString timestamp = line.timestamp().toString(timestampFormat_);
    QString text = line.text();

    int xTextStart = 0;

    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, consoleView_->palette().highlight());
    }
    else if (backgroundColor.isValid())
    {
        painter->fillRect(option.rect, backgroundColor);
    }

    painter->setFont(consoleView_->font());
    painter->setPen(QColor(Qt::white).darker(150));

    if (consoleView_->timestampsEnabled())
    {
        painter->setPen(QColor(Qt::white).darker(125));
        painter->drawText(adjusted, Qt::AlignLeft, timestamp);
        painter->setPen(QColor("white").darker(125)); // FIXME: only required if background is repainted
        painter->drawLine(timestampWidth()+2,0,timestampWidth()+2,option.rect.bottom()); // FIXME: only required if background is repainted
        xTextStart += timestampWidth() + 7;
    }

    adjusted.setLeft(xTextStart);

    // draw text
    painter->setPen(consoleView_->textColor());

    int numChars = charsPerLine_;
    int numLines = 1;
    if (numChars > 0)
        numLines = line.text().length() / numChars + 1;

    int lastLineWidth = 0;
    for (int i = 0; i < numLines; i++)
    {
        QString lineText = text.mid(i*numChars, numChars);
        painter->drawText(adjusted.x(), option.rect.y()+ (fontHeight_+1) + (i)*(fontHeight_+2)-metrics.descent(),lineText);

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

    painter->restore();
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
