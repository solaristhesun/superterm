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

    int timestampWidth() const;

    void updateFontMetrics();

private:
    int charsPerLine(const int width) const;

private:
    ConsoleView* consoleView_;
    QString      timestampFormat_;
    int          timestampWidth_;
    int          cursorWidth_;
    int          fontHeight_;
    int          fontDescent_;
    int          charsPerLine_;
};

#endif // CONSOLELINEITEMDELEGATE_H
