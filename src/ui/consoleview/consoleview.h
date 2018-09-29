#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QListView>
#include <QKeyEvent>

namespace Ui {
class ConsoleView;
}

class CConsoleTab;
class ConsoleLineItemDelegate;

class ConsoleView : public QListView
{
    Q_OBJECT

public:
    explicit ConsoleView(QWidget *parent = nullptr);
    ~ConsoleView();
    void keyPressEvent(QKeyEvent* e);

    void paintEvent(QPaintEvent *event);
    QSize getCharWidth() const;

    void setTimestampsEnabled(const bool bTimestampsEnabled);
    bool timestampsEnabled() const;

    void setTextColor(QColor color);
    QColor textColor() const;

    void setBackgroundColor(QColor color);
    QColor backgroundColor() const;


signals:
    void keyPressed(QKeyEvent* e);

private:
    void drawTimestampsArea();

private:
    Ui::ConsoleView*         ui_;
    CConsoleTab*             consoleTab_;
    bool                     bTimestampsEnabled_;
    QColor                   textColor_;
    QColor                   backgroundColor_;
};

#endif // CONSOLEVIEW_H
