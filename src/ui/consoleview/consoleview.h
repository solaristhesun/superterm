#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QKeyEvent>
#include <QListView>
#include <QObject>

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
    ~ConsoleView();

    void paintEvent(QPaintEvent* event) override;

    void setModel(QAbstractItemModel* model) override;

    void resizeEvent(QResizeEvent *event) override;

    void mouseReleaseEvent(QMouseEvent * event) override;

    void mousePressEvent(QMouseEvent * event) override;

    void setFont(const QFont & font);

    QSize getCharWidth() const;

    void setTimestampsEnabled(const bool bTimestampsEnabled);
    bool timestampsEnabled() const;

    void   setTextColor(QColor color);
    QColor textColor() const;

    void   setBackgroundColor(QColor color);
    QColor backgroundColor() const;

    void setAutoScrollToBottom(const bool bAutoScrollToBottom);
    bool autoScrollToBottom() const;

public slots:
    void onRowsInserted(QModelIndex, int, int);

private:
    void drawTimestampsArea();
    void copySelectionToClipboard();

private:
    Ui::ConsoleView*         ui_;
    ConsoleLineItemDelegate* itemDelegate_;
    bool                     bTimestampsEnabled_;
    bool                     bAutoScrollToBottom_;
    QColor                   textColor_;
    QColor                   backgroundColor_;
};

#endif // CONSOLEVIEW_H
