#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class CConsoleTab;
class CConsoleTabBar;
class CPortEnumerator;
class CSession;
class CMainWindow;
class QMenu;

class CConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CConsoleTabWidget(QWidget* parent = nullptr);
    ~CConsoleTabWidget();

    void         addTab(CConsoleTab* tab);
    void         setCurrentTabText(const QString& test);
    void         setConsoleFont(const QFont& font);
    void         aboutToQuit();
    void         destroyTab(int index);
    CConsoleTab* currentWidget() const;
    CConsoleTab* widget(int index) const;

public slots:
    void closeTab(int index);
    void onAddButtonClicked();
    void onTabDetached(int index);
    void showContextMenu(const QPoint& pt);
    void showTabRenameDialog();

private:
    void createContextMenu();

private:
    CConsoleTabBar* m_tabBar;
    QMenu*          m_contextMenu;
    QAction*        m_renameTabAction;
};

#endif // CONSOLETABWIDGET_H
