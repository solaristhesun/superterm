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

class CConsoleTab : public QWidget
{
    Q_OBJECT

public:
    explicit CConsoleTab(CConsoleTabWidget *parent = 0);
    ~CConsoleTab();

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
    void showError(QSerialPort::SerialPortError error);
    void updateHighlighting(void);

private:
    Ui::CConsoleTab*   m_ui;
    CConsoleTabWidget* m_parent;
    QSerialPort*      m_port;
    int               m_lastTabIndex;

    static quint32    m_u32counter;
    QStringList       m_highlightings;

    void fillComboBoxes();
};

#endif // CONSOLETAB_H
