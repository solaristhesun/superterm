#ifndef CONSOLETAB_H
#define CONSOLETAB_H

#include <QProcess>
#include <QSerialPort>
#include <QTextEdit>
#include <QWidget>

namespace Ui
{
class CConsoleTab;
}

class QSerialPort;
class ConsoleLineBuffer;
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
    CConsoleTab(CPortEnumerator* pe, CSession* session = nullptr);
    ~CConsoleTab();

    void hideButton(int index);

    void    setConsoleFont(const QFont& font);
    void    setColor(const QColor& backGroundColor, const QColor& foreGroundColor);
    QString getLabel() const;
    void    setLabel(const QString& label);
    QSize   getViewPortSize() const;
    QSize   getCharWidth() const;
    void    disconnectEndpoint();

    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void onConnectClicked();
    void onKeyPressed(QKeyEvent* e);
    void showContextMenu(const QPoint& pt);
    void showColorDialog();
    void showFontDialog();
    void showFontColorDialog();
    void showError(QSerialPort::SerialPortError error);
    void updateHighlightings();
    void showAboutDialog();
    void toggleFullScreen();
    void toggleAutoScroll();
    void startLogging();
    void stopLogging();
    void showSaveDialog();
    void onEndpointData(const CMessage& message);
    void onEndpointConnected();
    void onEndpointDisconnected(int returnCode);
    void onReconnectionSignal(const CMessage& message);
    void onReconnectionCancel();
    void onConfigurationChanged(const QString& config);
    void showRenameTabDialog();
    void onRenameTab();
    void clearTab();
    void toggleTimeStamps();

signals:
    void labelChanged(const QString&);

private:
    Ui::CConsoleTab*   m_ui;
    CMainWindow*       mMainWindow;
    ConsoleLineBuffer* lineBuffer_;
    QString            mTabLabel;
    CPortEndpoint*     m_portEndpoint;
    CSession*          m_session;
    QFile*             m_logFile;
    QMenu*             m_contextMenu;
    int                m_lastTabIndex;

    static quint32 m_u32counter;
    QStringList    m_highlightings;

    void fillComboBoxes();
    void createContextMenu();
    void escapeSpecialChars(QByteArray& data);
    void destroySession();
};

#endif // CONSOLETAB_H
