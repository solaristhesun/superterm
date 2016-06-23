#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QWidget>
#include <QSerialPort>
#include <QTextEdit>
#include <QProcess>

namespace Ui {
class CConsoleTab;
}

class QSerialPort;
class CConsoleTabWidget;
class QFile;
class CPortEnumerator;
class QMenu;
class CPortEndpoint;
class CSession;

class CConsoleTab : public QWidget
{
    Q_OBJECT

public:
    explicit CConsoleTab(CPortEnumerator* pe, CConsoleTabWidget *parent = 0, CSession* session = NULL);
    ~CConsoleTab();

    void hideButton(int index);

    void setConsoleFont(const QFont& font);
    void setBackgroundColor(const QColor& color);

public slots:
    void onConnectClicked(void);
    void onKeyPressed(QKeyEvent* e);
    void onConfigurationChanged(const QString &text);
    void onComboChanged(void);
    void onMoreClicked(void);
    void showContextMenu(const QPoint &pt);
    void showColorDialog(void);
    void showConnectBar(void);
    void hideConnectBar(void);
    void showFontDialog(void);
    void showError(QSerialPort::SerialPortError error);
    void updateHighlighting(void);
    void showAboutDialog(void);
    void toggleFullScreen(void);
    void toggleAutoScroll(void);
    void startLogging(void);
    void stopLogging(void);
    void showSaveDialog(void);
    void onAppQuit(void);
    void onEndpointData();
    void onEndpointConnected();
    void onEndpointDisconnected();

private:
    Ui::CConsoleTab*   m_ui;
    CPortEndpoint*     m_portEndpoint;
    CPortEnumerator*   m_pe;
    CConsoleTabWidget* m_parent;
    CSession*          m_session;
    QFile*             m_logFile;
    QMenu*             m_menu;
    int                m_lastTabIndex;

    static quint32     m_u32counter;
    QStringList        m_highlightings;

    void fillComboBoxes();
    void createContextMenu();
};

#endif // CONSOLETAB_H
