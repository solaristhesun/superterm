#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QTextEdit>

namespace Ui {
	class ConsoleView;
}

class ConsoleView : public QTextEdit
{
    Q_OBJECT
    
public:
    explicit ConsoleView(QWidget *parent = 0);
    ~ConsoleView();

    void keyPressEvent(QKeyEvent * e);

private:
    Ui::ConsoleView *ui;
};

#endif // CONSOLEVIEW_H
