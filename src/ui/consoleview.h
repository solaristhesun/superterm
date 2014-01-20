#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QTextEdit>

namespace Ui {
	class ConsoleView;
}

class QSerialPort;

class ConsoleView : public QTextEdit
{
    Q_OBJECT
    
public:
    ConsoleView(QSerialPort *port, QWidget *parent = 0);
    ~ConsoleView();

    void keyPressEvent(QKeyEvent * e);

public slots:
    void showData(void);

private:
    Ui::ConsoleView *ui;
    QSerialPort* m_port;
};

#endif // CONSOLEVIEW_H
