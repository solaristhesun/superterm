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
class CMainWindow;
class CMessage;

class CConsoleTab : public QWidget
{
    Q_OBJECT

public:
    CConsoleTab(CPortEnumerator* pe, CSession* session = NULL);
    ~CConsoleTab();

    void hideButton(int index);

    void setConsoleFont(const QFont& font);
    void setBackgroundColor(const QColor& color);
    QString getLabel() const;
    void setLabel(const QString& label);

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
    void onEndpointData(const CMessage& message);
    void onEndpointConnected();
    void onEndpointDisconnected();
    void onReconnectionSignal(const CMessage& message);
    void onReconnectionCancel();

signals:
    void labelChanged(const QString&);

private:
    Ui::CConsoleTab*   m_ui;
    CMainWindow*       mMainWindow;
    QString            mTabLabel;
    CPortEndpoint*     m_portEndpoint;
    CPortEnumerator*   m_pe;
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
