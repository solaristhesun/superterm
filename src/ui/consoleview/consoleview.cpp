#include <QPainter>
#include <QTimer>
#include <QClipboard>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QDebug>

#include "ui_consoleview.h"
#include "models/consoleline.h"
#include "ui/consoleview/consoleview.h"
#include "ui/consoleview/consolelineitemdelegate.h"

static QElapsedTimer timer;  // FIXME: remove

ConsoleView::ConsoleView(QWidget *parent)
    : QListView(parent)
    , ui_(new Ui::ConsoleView)
    , itemDelegate_(new ConsoleLineItemDelegate(this))
    , backgroundBrush_(Qt::SolidPattern)
    , timestampBrush_(Qt::SolidPattern)
    , timestampPen_(Qt::SolidLine)
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

    painter.fillRect(event->rect(), backgroundBrush_);

    if (bTimestampsEnabled_)
    {
        paintTimestampsArea(painter, event->rect());
    }

    timer.start();
    QListView::paintEvent(event);
    qDebug() << "paintEvent" << timer.elapsed() << "ms";
}

void ConsoleView::paintTimestampsArea(QPainter& painter, const QRect& rect)
{
    const int timestampWidth = itemDelegate_->timestampWidth()+2;

    // fill area
    painter.fillRect(rect.x(), rect.y(), timestampWidth, rect.height(), timestampBrush_);

    // draw line
    painter.setPen(timestampPen_);
    painter.drawLine(timestampWidth, rect.y(), timestampWidth, rect.height());
}

void ConsoleView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);

    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(onRowsInserted(QModelIndex,int,int)), Qt::UniqueConnection);
}

void ConsoleView::resizeEvent(QResizeEvent *event)
{
    qDebug() << "ConsoleView::resizeEvent" << event->size();

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
    itemDelegate_->updateFontMetrics();
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

void ConsoleView::setTextColor(const QColor& color)
{
    textColor_ = color;
    timestampPen_.setColor(color.darker(150));
}

QColor ConsoleView::textColor() const
{
    return textColor_;
}

void ConsoleView::setBackgroundColor(const QColor& color)
{
    backgroundBrush_.setColor(color);
    timestampBrush_.setColor(color.darker(120));
}

QColor ConsoleView::backgroundColor() const
{
    return backgroundBrush_.color();
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
        ConsoleLine line = QListView::model()->data(index).value<ConsoleLine>();

        if (bTimestampsEnabled_)
        {
            copyText += line.timestamp().toString("[yyyy-MM-dd HH:mm:ss.zzz] ");
        }

        copyText += line.text() + "\r\n";
    }

    QApplication::clipboard()->setText(copyText.trimmed());
}

// EOF <stefan@scheler.com>
