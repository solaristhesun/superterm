#include "portitemdelegate.h"

#include <QPainter>

PortItemDelegate::PortItemDelegate()
    : view_(new PortItemView(nullptr))
{

}


void PortItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPalette pal;

    if (option.state & QStyle::State_Selected)
    {
        pal.setColor(QPalette::Background, QColor("#9dc1ca"));
    }
    else
    {
        pal.setColor(QPalette::Background, QColor("white"));
    }

    QString text = index.data().toString();

    if (text.contains("Port"))
    {
        QItemDelegate::paint(painter, option, index);
        return;
    }


    view_->setPortName(index.data().toString());

    painter->save();
    painter->translate(option.rect.topLeft());
    view_->render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}

QSize PortItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QString text = index.data().toString();

    if (text.contains("Port"))
    {
        return QItemDelegate::sizeHint( option, index );
    }

    return QSize(100, 38);
}
