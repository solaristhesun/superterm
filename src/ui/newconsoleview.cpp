#include <QItemDelegate>
#include <QPainter>
#include <QDebug>

#include "newconsoleview.h"
#include "ui_newconsoleview.h"
#include "ui/consoletab.h"

#include "models/consoleline.h"


class ConsoleLineItemDelegate : public QItemDelegate
{
public:
    ConsoleLineItemDelegate(QObject *parent)
        : QItemDelegate(parent)
        , font_("Consolas", 11)
    {

    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {


        QFontMetrics metrics(font_);
        return QSize(1,metrics.height()+4);
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QFontMetrics metrics(font_);

        const int cursorWidth = metrics.width(" ");

        QRect adjusted = option.rect.adjusted(2,+2,-2,-2);

        ConsoleLine line = qvariant_cast<ConsoleLine>(index.data());
        QColor backgroundColor = line.color();
        painter->save();

        QString timestamp = "[" + line.timestamp().toString("yyyy-MM-dd HH:mm:ss.zzz") + "]";
        QString text = line.text();

        int widthTimeStamp = metrics.width("[yyyy-MM-dd HH:mm:ss.zzz]");
        // draw background color
        if (backgroundColor.isValid())
        {
            painter->setPen(QColor(backgroundColor));
            painter->setBrush(QBrush(QColor(backgroundColor)));
            painter->drawRect(option.rect);
        }

         painter->setFont(font_);
         painter->setPen(QColor(Qt::white).darker(150));

        if (line.timestamp().isValid())
        {
            QColor c("#142462");
            painter->setPen(QColor(c.darker(120)));
            painter->setBrush(QBrush(QColor(c.darker(120))));
            painter->drawRect(0,option.rect.y(),widthTimeStamp+7,option.rect.height());
            painter->setPen(QColor(Qt::white).darker(150));
            painter->drawText(adjusted, Qt::AlignLeft, timestamp);
            painter->drawLine(widthTimeStamp + 7, option.rect.y(), widthTimeStamp+7, option.rect.bottom());
        }

        adjusted.setLeft(widthTimeStamp + 15);
        painter->setPen(Qt::white);
        painter->drawText(adjusted, Qt::AlignLeft, text);
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        if (index.row() == index.model()->rowCount()-1)
            painter->drawRect(widthTimeStamp + 15 + metrics.width(text) + 1, option.rect.y()+2, cursorWidth, option.rect.height()-4);

        painter->restore();
    }

private:
    QFont font_;
};

NewConsoleView::NewConsoleView(QWidget *parent)
    : QListView(parent)
    , ui(new Ui::NewConsoleView)
    , parent_(static_cast<CConsoleTab*>(parent))
{
    ui->setupUi(this);

    ConsoleLineItemDelegate* delegate = new ConsoleLineItemDelegate(this);
    QListView::setItemDelegate(delegate);


    connect(model(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(scrollToBottom()));
}

NewConsoleView::~NewConsoleView()
{
    delete ui;
}

void NewConsoleView::keyPressEvent(QKeyEvent* e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        parent_->toggleFullScreen();
        return;
    }

    emit keyPressed(e);
}
