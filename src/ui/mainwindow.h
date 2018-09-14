#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CConsoleTab;
class SoftwareVersion;

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

private:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

public slots:
    void onSecondaryInstanceLaunched();
    void showUpdateInfo(const SoftwareVersion& version);

signals:
    void willQuit();

private:
    Ui::CMainWindow* m_ui;
};

#endif // MAINWINDOW_H
