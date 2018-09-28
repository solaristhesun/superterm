#ifndef NEWCONSOLEVIEW_H
#define NEWCONSOLEVIEW_H

#include <QListView>
#include <QKeyEvent>

namespace Ui {
class NewConsoleView;
}

class CConsoleTab;

class NewConsoleView : public QListView
{
    Q_OBJECT

public:
    explicit NewConsoleView(QWidget *parent = nullptr);
    ~NewConsoleView();
    void keyPressEvent(QKeyEvent* e);

signals:
    void keyPressed(QKeyEvent* e);

private:
    Ui::NewConsoleView *ui;
    CConsoleTab*        parent_;
};

#endif // NEWCONSOLEVIEW_H
