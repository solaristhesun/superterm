#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

class ConsoleMgr;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showAbout(void);
    void connectPort(void);
    void closeTab(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
