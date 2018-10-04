#include <QPainter>
#include <QTimer>
#include <QClipboard>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QDebug>

#include "consoleview.h"
#include "ui_consoleview.h"
#include "models/consoleline.h"
#include "ui/consoleview/consolelineitemdelegate.h"

ConsoleView::ConsoleView(QWidget *parent)
    : QListView(parent)
    , ui_(new Ui::ConsoleView)
    , itemDelegate_(new ConsoleLineItemDelegate(this))
    , bTimestampsEnabled_(false)
    , bAutoScrollToBottom_(true)
    , bAutoScrollTriggered(false)
{
    ui_->setupUi(this);
    itemDelegate_->updateFontMetrics();

    QListView::setItemDelegate(itemDelegate_);
}

ConsoleView::~ConsoleView()
{
    delete ui_;
}

void ConsoleView::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        QListView::mouseReleaseEvent(event);
        copySelectionToClipboard();
        QListView::clearSelection();
    }
    else
    {
        // forward directly to QAbstractScrollArea to prevent right click selection
        QAbstractScrollArea::mouseReleaseEvent(event);
    }
}

void ConsoleView::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::RightButton)
    {
        // forward directly to QAbstractScrollArea to prevent right click selection
        QAbstractScrollArea::mousePressEvent(event);
    }
    else
    {
        QListView::mousePressEvent(event);
    }
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
        int widthTimeStamp = itemDelegate_->timestampWidth();

        painter.setPen(backgroundColor().darker(120));
        painter.setBrush(QBrush(QColor(backgroundColor().darker(120))));
        painter.drawRect(0,cr.y(),widthTimeStamp+2,cr.height());
        painter.setPen(QColor("white").darker(150));
        painter.drawLine(widthTimeStamp+2, cr.y(), widthTimeStamp+2, cr.bottom());
    }

    QElapsedTimer timer; timer.start();
    QListView::paintEvent(event);
    qDebug() << "paintEvent" << timer.elapsed() << "ms";
}

void ConsoleView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);

    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(onRowsInserted(QModelIndex,int,int)), Qt::UniqueConnection);
}

void ConsoleView::resizeEvent(QResizeEvent *event)
{
    qDebug() << "RESIZE";

    QListView::reset(); // FIXME: check why this is necessary
    QListView::resizeEvent(event);
    itemDelegate_->updateFontMetrics();
}

void ConsoleView::setFont(const QFont& font)
{
    QListView::setFont(font);
    itemDelegate_->updateFontMetrics();
}

void ConsoleView::onRowsInserted(QModelIndex,int,int)
{
    if (bAutoScrollToBottom_)
    {
        //QListView::scrollToBottom();
        if (!bAutoScrollTriggered)
        {
            bAutoScrollTriggered = true;
            QTimer::singleShot(50, this, &ConsoleView::scrollToBottom);
        }
    }
}

void ConsoleView::scrollToBottom()
{
    QListView::scrollToBottom();
    bAutoScrollTriggered = false;
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
    int widthTimeStamp = delegate->timestampWidth();

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

void ConsoleView::copySelectionToClipboard()
{
    QString copyText;
    QModelIndexList list = QListView::selectedIndexes();

    for (QModelIndex& index : list)
    {
        ConsoleLine line = qvariant_cast<ConsoleLine>(QListView::model()->data(index));

        if (bTimestampsEnabled_)
        {
            copyText += line.timestamp().toString("[yyyy-MM-dd HH:mm:ss.zzz] ");
        }

        copyText += line.text() + "\r\n";
    }

    QApplication::clipboard()->setText(copyText.trimmed());
}

// EOF <stefan@scheler.com>
