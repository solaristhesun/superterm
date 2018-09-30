#include <QPainter>
#include <QDebug>

#include "consoleview.h"
#include "ui_consoleview.h"
#include "models/consoleline.h"
#include "ui/consoleview/consolelineitemdelegate.h"

ConsoleView::ConsoleView(QWidget *parent)
    : QListView(parent)
    , ui_(new Ui::ConsoleView)
    , bTimestampsEnabled_(false)
    , bAutoScrollToBottom_(true)
{
    ui_->setupUi(this);

    QListView::setItemDelegate(new ConsoleLineItemDelegate(this));
}

ConsoleView::~ConsoleView()
{
    delete ui_;
}

void ConsoleView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QRect cr = rect();

    painter.setPen(backgroundColor());
    painter.setBrush(QBrush(backgroundColor()));
    painter.drawRect(0, cr.y(), cr.width(), cr.height());

    if (bTimestampsEnabled_)
    {
        ConsoleLineItemDelegate* delegate = static_cast<ConsoleLineItemDelegate*>(itemDelegate());
        int widthTimeStamp = delegate->getTimestampWidth();

        painter.setPen(backgroundColor().darker(120));
        painter.setBrush(QBrush(QColor(backgroundColor().darker(120))));
        painter.drawRect(0,cr.y(),widthTimeStamp+5,cr.height());
        painter.setPen(QColor("white").darker(150));
        painter.drawLine(widthTimeStamp + 5, cr.y(), widthTimeStamp+5, cr.bottom());
    }

    QListView::paintEvent(event);
}

void ConsoleView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);

    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(onRowsInserted(QModelIndex,int,int)), Qt::UniqueConnection);
}

void ConsoleView::resizeEvent(QResizeEvent *event) {

    QListView::reset(); // FIXME: check why this is necessary
    QListView::resizeEvent(event);
}

void ConsoleView::onRowsInserted(QModelIndex,int,int)
{
    if (bAutoScrollToBottom_)
    {
        QListView::scrollToBottom();
    }
}

void ConsoleView::setTimestampsEnabled(const bool bTimestampsEnabled)
{
    bTimestampsEnabled_ = bTimestampsEnabled;
}

bool ConsoleView::timestampsEnabled() const
{
    return bTimestampsEnabled_;
}

void ConsoleView::setAutoScrollToBottom(const bool bAutoScrollToBottom)
{
    bAutoScrollToBottom_ = bAutoScrollToBottom;

    if (bAutoScrollToBottom)
    {
        QListView::scrollToBottom();
    }
}

bool ConsoleView::autoScrollToBottom() const
{
    return bAutoScrollToBottom_;
}

void ConsoleView::setTextColor(QColor color)
{
    textColor_ = color;
}

QColor ConsoleView::textColor() const
{
    return textColor_;
}

void ConsoleView::setBackgroundColor(QColor color)
{
    backgroundColor_ = color;
}

QColor ConsoleView::backgroundColor() const
{
    return backgroundColor_;
}

void ConsoleView::drawTimestampsArea()
{
    QPainter painter(viewport());
    QRect cr = contentsRect();
    ConsoleLineItemDelegate* delegate = static_cast<ConsoleLineItemDelegate*>(itemDelegate());
    int widthTimeStamp = delegate->getTimestampWidth();

    QColor c("#142462");
    painter.setPen(QColor(c.darker(120)));
    painter.setBrush(QBrush(QColor(c.darker(120))));
    //painter.drawRect(0,cr.y(),widthTimeStamp+5,cr.height());
    painter.setPen(backgroundColor());
    painter.setBrush(QBrush(backgroundColor()));
    //painter.drawRect(widthTimeStamp+6, cr.y(), cr.width(), cr.height());
    painter.setPen(QColor("white").darker(150));
    //painter.drawLine(widthTimeStamp + 5, cr.y(), widthTimeStamp+5, cr.bottom());
}

QSize ConsoleView::getCharWidth() const
{
    QFontMetrics fm(font());
    return QSize(fm.averageCharWidth(), fm.height());
}
