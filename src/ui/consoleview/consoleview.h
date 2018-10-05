#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QKeyEvent>
#include <QListView>
#include <QObject>
#include <QPen>

namespace Ui
{
class ConsoleView;
}

class ConsoleLineItemDelegate;

class ConsoleView : public QListView
{
    Q_OBJECT

public:
    explicit ConsoleView(QWidget* parent = nullptr);
    ~ConsoleView() override;

    void paintEvent(QPaintEvent* event) override;

    void setModel(QAbstractItemModel* model) override;

    void resizeEvent(QResizeEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void setFont(const QFont& font);

    QSize getCharWidth() const;

    void setTimestampsEnabled(const bool bTimestampsEnabled);
    bool timestampsEnabled() const;

    void   setTextColor(const QColor& color);
    QColor textColor() const;

    void   setBackgroundColor(const QColor& color);
    QColor backgroundColor() const;

    void setAutoScrollToBottom(const bool bAutoScrollToBottom);
    bool autoScrollToBottom() const;

public slots:
    void onRowsInserted(QModelIndex, int, int);
    void scrollToBottom();

private:
    void paintTimestampsArea(QPainter& painter, const QRect& rect);
    void copySelectionToClipboard();

private:
    Ui::ConsoleView*         ui_;
    ConsoleLineItemDelegate* itemDelegate_;
    QBrush                   backgroundBrush_;
    QBrush                   timestampBrush_;
    QPen                     timestampPen_;
    bool                     bTimestampsEnabled_;
    bool                     bAutoScrollToBottom_;
    QColor                   textColor_;
    bool                     bAutoScrollTriggered;
};

#endif // CONSOLEVIEW_H
