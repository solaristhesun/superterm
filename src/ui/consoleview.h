#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QTextEdit>

namespace Ui {
    class ConsoleView;
}

class ConsoleTab;

class ConsoleView : public QTextEdit
{
    Q_OBJECT
    
public:
    ConsoleView(QWidget *parent = 0);
    ~ConsoleView();

    void keyPressEvent(QKeyEvent * e);
    void scrollDown(void);

public slots:


signals:
    void keyPressed(QString text);

private:
    Ui::ConsoleView* m_ui;
    ConsoleTab*      m_parent;

};

#endif // CONSOLEVIEW
