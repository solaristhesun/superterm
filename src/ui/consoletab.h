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
    void onConnectClicked();
    void onKeyPressed(QKeyEvent* e);
    void showContextMenu(const QPoint& pt);
    void showColorDialog();
    void showFontDialog();
    void showError(QSerialPort::SerialPortError error);
    void updateHighlighting();
    void showAboutDialog();
    void toggleFullScreen();
    void toggleAutoScroll();
    void startLogging();
    void stopLogging();
    void showSaveDialog();
    void onAppQuit();
    void onEndpointData(const CMessage& message);
    void onEndpointConnected();
    void onEndpointDisconnected(int returnCode);
    void onReconnectionSignal(const CMessage& message);
    void onReconnectionCancel();
    void onConfigurationChanged(const QString& config);

signals:
    void labelChanged(const QString&);

private:
    Ui::CConsoleTab* m_ui;
    CMainWindow*     mMainWindow;
    QString          mTabLabel;
    CPortEndpoint*   m_portEndpoint;
    CSession*        m_session;
    QFile*           m_logFile;
    QMenu*           m_menu;
    int              m_lastTabIndex;

    static quint32 m_u32counter;
    QStringList    m_highlightings;

    void fillComboBoxes();
    void createContextMenu();
};

#endif // CONSOLETAB_H
