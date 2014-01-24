#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QTextEdit>

namespace Ui {
	class ConsoleView;
}

class QSerialPort;
class QTabWidget;

class ConsoleView : public QTextEdit
{
    Q_OBJECT
    
public:
    ConsoleView(QSerialPort *port, QTabWidget *parent = 0);
    ~ConsoleView();

    void keyPressEvent(QKeyEvent * e);

public slots:
    void showData(void);

private:
    Ui::ConsoleView *ui;
    QSerialPort*    m_port;
    QTabWidget*     m_parent;
    int             m_lastTabIndex;

    void scrollDown(void);
    void toggleFullScreen(void);
};

#endif // CONSOLEVIEW_H
