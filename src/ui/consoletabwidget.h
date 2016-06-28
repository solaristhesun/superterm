#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class CConsoleTab;
class CConsoleTabBar;
class CPortEnumerator;
class CSession;
class CMainWindow;

class CConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CConsoleTabWidget(QWidget *parent = 0);
    ~CConsoleTabWidget();

    void setCurrentTabText(const QString& test);
    void setConsoleFont(const QFont& font);

    void aboutToQuit(void);

signals:
    void appQuits(void);

public slots:
    void closeTab(int index);
    void handleAddButtonClicked(void);
    void addNewTab(CSession* session);
    void onTabDetached(int index);

private:
    CConsoleTabBar*  m_tabBar;
    CPortEnumerator* m_pe;
};

#endif // CONSOLETABWIDGET_H
