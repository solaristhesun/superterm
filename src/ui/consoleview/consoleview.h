#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QListView>
#include <QKeyEvent>

namespace Ui {
class ConsoleView;
}

class CConsoleTab;

class ConsoleView : public QListView
{
    Q_OBJECT

public:
    explicit ConsoleView(QWidget *parent = nullptr);
    ~ConsoleView();
    void keyPressEvent(QKeyEvent* e);

    void paintEvent(QPaintEvent *event);
    QSize getCharWidth() const;

signals:
    void keyPressed(QKeyEvent* e);

private:
    Ui::ConsoleView *ui;
    CConsoleTab*        parent_;
};

#endif // CONSOLEVIEW_H
