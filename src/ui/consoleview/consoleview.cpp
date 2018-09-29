#include <QPainter>
#include <QDebug>

#include "consoleview.h"
#include "ui_consoleview.h"
#include "ui/consoletab.h"
#include "models/consoleline.h"
#include "ui/consoleview/consolelineitemdelegate.h"

ConsoleView::ConsoleView(QWidget *parent)
    : QListView(parent)
    , ui_(new Ui::ConsoleView)
    , consoleTab_(static_cast<CConsoleTab*>(parent))
    , bTimestampsEnabled_(false)
{
    ui_->setupUi(this);

    QListView::setItemDelegate(new ConsoleLineItemDelegate(this));

    connect(model(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(scrollToBottom()));
}

ConsoleView::~ConsoleView()
{
    delete ui_;
}

void ConsoleView::keyPressEvent(QKeyEvent* e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        consoleTab_->toggleFullScreen();
        return;
    }

    emit keyPressed(e);
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
        painter.drawRect(0,cr.y(),widthTimeStamp+7,cr.height());
        painter.setPen(QColor("white").darker(150));
        painter.drawLine(widthTimeStamp + 7, cr.y(), widthTimeStamp+7, cr.bottom());
    }

    QListView::paintEvent(event);
}

void ConsoleView::setTimestampsEnabled(const bool bTimestampsEnabled)
{
    bTimestampsEnabled_ = bTimestampsEnabled;
}

bool ConsoleView::timestampsEnabled() const
{
    return bTimestampsEnabled_;
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
    painter.drawRect(0,cr.y(),widthTimeStamp+7,cr.height());
    painter.setPen(backgroundColor());
    painter.setBrush(QBrush(backgroundColor()));
    painter.drawRect(widthTimeStamp+8, cr.y(), cr.width(), cr.height());
    painter.setPen(QColor("white").darker(150));
    painter.drawLine(widthTimeStamp + 7, cr.y(), widthTimeStamp+7, cr.bottom());
}

QSize ConsoleView::getCharWidth() const
{
    QFontMetrics fm(font());
    return QSize(fm.averageCharWidth(), fm.height());
}
