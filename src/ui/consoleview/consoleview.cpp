#include <QItemDelegate>
#include <QPainter>
#include <QDebug>

#include "consoleview.h"
#include "ui_consoleview.h"
#include "ui/consoletab.h"
#include "models/consoleline.h"
#include "ui/consoleview/consolelineitemdelegate.h"

ConsoleView::ConsoleView(QWidget *parent)
    : QListView(parent)
    , ui(new Ui::ConsoleView)
    , parent_(static_cast<CConsoleTab*>(parent))
{
    ui->setupUi(this);

    QListView::setItemDelegate(new ConsoleLineItemDelegate(this));

    connect(model(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(scrollToBottom()));
}

ConsoleView::~ConsoleView()
{
    delete ui;
}

void ConsoleView::keyPressEvent(QKeyEvent* e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        parent_->toggleFullScreen();
        return;
    }

    emit keyPressed(e);
}

void ConsoleView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QRect cr = contentsRect();
    int widthTimeStamp = 206;
    QColor c("#142462");
    painter.setPen(QColor(c.darker(120)));
    painter.setBrush(QBrush(QColor(c.darker(120))));
    painter.drawRect(0,cr.y(),widthTimeStamp+7,cr.height());
    painter.setPen(QColor("white").darker(150));
    painter.drawLine(widthTimeStamp + 7, cr.y(), widthTimeStamp+7, cr.bottom());

    QListView::paintEvent(event);
}

QSize ConsoleView::getCharWidth() const
{
    QFontMetrics fm(font());
    return QSize(fm.averageCharWidth(), fm.height());
}
