#ifndef PORTITEMDELEGATE_H
#define PORTITEMDELEGATE_H

#include <QItemDelegate>

class PortItemView;

class PortItemDelegate : public QItemDelegate
{
public:
    PortItemDelegate();

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    PortItemView* view_;
};

#endif // PORTITEMDELEGATE_H
