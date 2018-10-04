#ifndef CONSOLETABBAR_H
#define CONSOLETABBAR_H

#include <QTabBar>

class QPushButton;
class MainWindow;

class ConsoleTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit ConsoleTabBar(QWidget* parent = nullptr);
    ~ConsoleTabBar();

    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void showEvent(QShowEvent* event);
    void tabInserted(int index);
    void tabRemoved(int index);

    void moveButton();
    void moveTab(int from, int to);

    MainWindow* getNewMainWindow() const
    {
        return newMainWindow_;
    }
    QPoint getClickOffset() const
    {
        return offset_;
    }

signals:
    void addButtonClicked();
    void tabDetached(int index);

public slots:

private:
    QPushButton* addTabButton_;
    MainWindow*  newMainWindow_;
    QPoint       offset_;
};

#endif // CONSOLETABBAR_H
