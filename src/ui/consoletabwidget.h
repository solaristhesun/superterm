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
    explicit CConsoleTabWidget(QWidget* parent = 0);
    ~CConsoleTabWidget();

    void addTab(CConsoleTab* tab);
    void setCurrentTabText(const QString& test);
    void setConsoleFont(const QFont& font);
    void aboutToQuit();
    void destroyTab(int index);
    CConsoleTab* currentWidget() const;
    CConsoleTab* widget(int index) const;

signals:
    void appQuits();

public slots:
    void closeTab(int index);
    void onAddButtonClicked();
    void onTabDetached(int index);

private:
    CConsoleTabBar* m_tabBar;
};

#endif // CONSOLETABWIDGET_H
