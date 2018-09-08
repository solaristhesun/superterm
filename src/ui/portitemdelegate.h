#ifndef PORTITEMDELEGATE_H
#define PORTITEMDELEGATE_H

#include <QItemDelegate>

class PortItemDelegate : public QItemDelegate
{
public:
    PortItemDelegate();

    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PORTITEMDELEGATE_H
