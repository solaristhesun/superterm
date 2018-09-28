#include <QItemDelegate>
#include <QPainter>

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

        QRect adjusted = option.rect.adjusted(2,+2,-2,-2);

        ConsoleLine line = qvariant_cast<ConsoleLine>(index.data());
        QColor backgroundColor = line.color();
        painter->save();

        QDateTime timestamp = line.timestamp();
        QString text = "[" + timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz") + "] " + line.text();

        if (backgroundColor.isValid())
        {
            painter->setPen(QColor(backgroundColor));
            painter->setBrush(QBrush(QColor(backgroundColor)));
            painter->drawRect(option.rect);
        }

        painter->setPen(Qt::white);
        painter->setFont(font_);
        painter->drawText(adjusted, Qt::AlignLeft, text);
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        if (index.row() == index.model()->rowCount()-1)
            painter->drawRect(metrics.width(text) + 4, option.rect.y()+2, 10, option.rect.height()-4);
        //qDebug() << metrics.width(line) << option.rect.y() <<metrics.width(line) + 10 << option.rect.height();
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
