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
    explicit MainWindow(ConsoleMgr& m_consoleMgr, QWidget *parent = 0);
    ~MainWindow();
    void	contextMenuEvent ( QContextMenuEvent * event ) { qDebug() << "BAZINGA! MENU!"; };
    
public slots:
    void showAbout(void);
    void connectPort(void);
    void closeTab(void);

private:
    Ui::MainWindow *ui;
    ConsoleMgr& m_consoleMgr;
};

#endif // MAINWINDOW_H
