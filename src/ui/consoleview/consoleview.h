#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QKeyEvent>
#include <QListView>
#include <QObject>

namespace Ui
{
class ConsoleView;
}

class CConsoleTab;
class ConsoleLineItemDelegate;

class ConsoleView : public QListView
{
    Q_OBJECT

public:
    explicit ConsoleView(QWidget* parent = nullptr);
    ~ConsoleView();

    void keyPressEvent(QKeyEvent* e) override;
    void paintEvent(QPaintEvent* event) override;

    void setModel(QAbstractItemModel* model) override;

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

signals:
    void keyPressed(QKeyEvent* e);

private:
    void drawTimestampsArea();

private:
    Ui::ConsoleView* ui_;
    CConsoleTab*     consoleTab_;
    bool             bTimestampsEnabled_;
    bool             bAutoScrollToBottom_;
    QColor           textColor_;
    QColor           backgroundColor_;
};

#endif // CONSOLEVIEW_H
