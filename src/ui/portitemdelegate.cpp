#include "portitemdelegate.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>

#include "serial/serialportinfo.h"

PortItemDelegate::PortItemDelegate()
    : view_(new PortItemView(nullptr))
{
    // currently emtpy
}


void PortItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    SerialPortInfo info = qvariant_cast<SerialPortInfo>(index.data(Qt::UserRole));

    QPalette pal;

    if (option.state & QStyle::State_Selected)
    {
        pal.setColor(QPalette::Background, option.palette.highlight().color());
    }
    else
    {
        pal.setColor(QPalette::Background, QColor("white"));
    }

    if (index.row() == 0)
    {
        QItemDelegate::paint(painter, option, index);
        return;
    }

    view_->setPortName(info.shortName());
    view_->setDescription("[" + info.description() + "]");
    view_->setBusyFlag(info.isBusy());
    view_->setPalette(pal);

    painter->save();
    painter->translate(option.rect.topLeft());
    view_->render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}

QSize PortItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return view_->size();
}
