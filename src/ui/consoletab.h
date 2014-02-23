#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QWidget>
#include <QSerialPort>
#include <QTextEdit>

namespace Ui {
class CConsoleTab;
}

class QSerialPort;
class CConsoleTabWidget;
class QFile;
class CPortEnumerator;
class QMenu;

class CConsoleTab : public QWidget
{
    Q_OBJECT

public:
    explicit CConsoleTab(CPortEnumerator* pe, CConsoleTabWidget *parent = 0);
    ~CConsoleTab();

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
    void showError(QSerialPort::SerialPortError error);
    void updateHighlighting(void);
    void showAboutDialog(void);
    void toggleFullScreen(void);
    void startLogging(void);
    void stopLogging(void);

private:
    Ui::CConsoleTab*   m_ui;
    CPortEnumerator*   m_pe;
    CConsoleTabWidget* m_parent;
    QSerialPort*       m_port;
    QFile*             m_logFile;
    QMenu*             m_menu;
    int                m_lastTabIndex;

    static quint32     m_u32counter;
    QStringList        m_highlightings;

    void fillComboBoxes();
    void createContextMenu();
};

#endif // CONSOLETAB_H
