#ifndef CONSOLELINEITEMDELEGATE_H
#define CONSOLELINEITEMDELEGATE_H

#include <QItemDelegate>

class ConsoleView;

class ConsoleLineItemDelegate : public QItemDelegate
{
public:
    explicit ConsoleLineItemDelegate(ConsoleView* parent);

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    int getTimestampWidth() const;

private:
    ConsoleView* consoleView_;
    QString      timestampFormat_;
};

#endif // CONSOLELINEITEMDELEGATE_H
