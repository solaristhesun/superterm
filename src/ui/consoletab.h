#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QWidget>

namespace Ui {
class ConsoleTab;
}

class QSerialPort;
class ConsoleTabWidget;

class ConsoleTab : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleTab(ConsoleTabWidget *parent = 0);
    ~ConsoleTab();

    void toggleFullScreen(void);
    void hideButton(int index);

    void setConsoleFont(const QFont& font);
    void setBackgroundColor(const QColor& color);

public slots:
    void onConnectClicked(void);
    void onDataAvailable(void);
    void onKeyPressed(QString text);
    void onComboChanged(void);
    void showContextMenu(const QPoint &pt);
    void showColorDialog(void);
    void showConnectBar(void);
    void showFontDialog(void);

private:
    Ui::ConsoleTab*   m_ui;
    ConsoleTabWidget* m_parent;
    QSerialPort*      m_port;
    int               m_lastTabIndex;

    static quint32    m_u32counter;

    void refreshPorts();
    void fillComboBoxes();
};

#endif // CONSOLETAB_H
