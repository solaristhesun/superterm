#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QWidget>

namespace Ui {
class ConsoleTab;
}

class QSerialPort;
class QTabWidget;

class ConsoleTab : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleTab(QTabWidget *parent = 0);
    ~ConsoleTab();

    void toggleFullScreen(void);

public slots:
    void onConnectClicked(void);
    void onDataAvailable(void);
    void onKeyPressed(QString text);

private:
    Ui::ConsoleTab *ui;
    QTabWidget*      m_parent;
    QSerialPort*     m_port;
    int              m_lastTabIndex;
};

#endif // CONSOLETAB_H
