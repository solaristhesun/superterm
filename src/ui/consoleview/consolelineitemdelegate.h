#ifndef CONSOLELINEITEMDELEGATE_H
#define CONSOLELINEITEMDELEGATE_H

#include <QItemDelegate>

class ConsoleLineItemDelegate : public QItemDelegate
{
public:
    explicit ConsoleLineItemDelegate(QObject *parent);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QFont font_;
};


#endif // CONSOLELINEITEMDELEGATE_H
