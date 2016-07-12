#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CConsoleTab;

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget* parent = 0);
    ~CMainWindow();

    QSize sizeHint() const;
    void attachTab(CConsoleTab* tab);
    void addExistingTabsFromFile();
    CConsoleTab* detachTab();
    int getTabCount() const;
    QRect getTabBarRect() const;

public slots:
    void aboutToQuit();

signals:
    void willQuit();

private:
    Ui::CMainWindow* m_ui;

    void removeTabFiles();
};

#endif // MAINWINDOW_H
