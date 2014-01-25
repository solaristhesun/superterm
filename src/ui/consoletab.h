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
    void onComboChanged(void);


private:
    Ui::ConsoleTab*  m_ui;
    QTabWidget*      m_parent;
    QSerialPort*     m_port;
    int              m_lastTabIndex;

    static quint32   m_u32counter;

    void refreshPorts();
};

#endif // CONSOLETAB_H
