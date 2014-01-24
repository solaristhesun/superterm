#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QTextEdit>

namespace Ui {
    class ConsoleTab;
}

class QSerialPort;
class QTabWidget;

class ConsoleTab : public QTextEdit
{
    Q_OBJECT
    
public:
    ConsoleTab(QSerialPort *port, QString title, QTabWidget *parent = 0);
    ~ConsoleTab();

    void keyPressEvent(QKeyEvent * e);

public slots:
    void showData(void);
    void showContextMenu(const QPoint &pt);
    void showColorDialog(void);

private:
    Ui::ConsoleTab* m_ui;
    QSerialPort*    m_port;
    QString         m_title;
    QTabWidget*     m_parent;
    int             m_lastTabIndex;

    void scrollDown(void);
    void toggleFullScreen(void);
};

#endif // CONSOLETAB_H
