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
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

    QSize sizeHint() const;
    void addTab(CConsoleTab* tab);
    void addExistingTabsFromFile(void);
    CConsoleTab* detachTab();
    int getTabCount() const;

protected:
    void moveEvent(QMoveEvent * event);

public slots:
    void aboutToQuit();

signals:
    void willQuit();

public:
    Ui::CMainWindow* m_ui; // FIXME: private

private:
    void removeTabFiles(void);
};

#endif // MAINWINDOW_H
