#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void addTab(QWidget* widget, QString& tabText);

protected:
    void moveEvent(QMoveEvent * event);

public slots:
    void aboutToQuit();

signals:
    void willQuit();

private:
    Ui::CMainWindow* m_ui;

    void addExistingTabsFromFile(void);
    void removeTabFiles(void);
};

#endif // MAINWINDOW_H
