#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class ConsoleTab;
class ConsoleTabBar;
class QMenu;

class ConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConsoleTabWidget(QWidget* parent = nullptr);
    ~ConsoleTabWidget();

    void        addTab(ConsoleTab* tab);
    void        setCurrentTabText(const QString& test);
    void        setConsoleFont(const QFont& font);
    void        aboutToQuit();
    void        destroyTab(int index);
    ConsoleTab* currentWidget() const;
    ConsoleTab* widget(int index) const;

public slots:
    void closeTab(int index);
    void onAddButtonClicked();
    void onTabDetached(int index);
    void showContextMenu(const QPoint& pt);
    void showTabRenameDialog();
    void onTabCloseRequested(int index);
    void activateNextTab();
    void activatePrevTab();

private:
    void createContextMenu();

private:
    ConsoleTabBar* tabBar_;
    QMenu*         contextMenu_;
    QAction*       renameTabAction_;
};

#endif // CONSOLETABWIDGET_H
