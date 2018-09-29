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

    void moveTab(int from, int to);

    void tabInserted(int index);
    void tabRemoved(int index);
    void moveButton();

    MainWindow* getNewMainWindow() const
    {
        return mNewMainWindow;
    }
    QPoint getClickOffset() const
    {
        return mOffset;
    }

signals:
    void addButtonClicked();
    void tabDetached(int index);

public slots:

private:
    QPushButton* m_btn;
    int          m_selectedIndex;
    int          m_prevIndex;
    MainWindow*  mNewMainWindow;
    QPoint       mOffset;
};

#endif // CONSOLETABBAR_H
