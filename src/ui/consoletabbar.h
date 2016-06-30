#ifndef CONSOLETABBAR_H
#define CONSOLETABBAR_H

#include <QTabBar>

class QPushButton;
class CMainWindow;

class CConsoleTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CConsoleTabBar(QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent* event);
    void showEvent(QShowEvent *event);

    void moveTab(int from, int to);


    void tabInserted(int index);
    void tabRemoved(int index);
    void moveButton(void);
    CMainWindow* getNewMainWindow() const { return mNewMainWindow; }
    QPoint getClickOffset() const { return mOffset; }

signals:
    void addButtonClicked(void);
    void tabDetached(int index);

public slots:

private:
    QPushButton *m_btn;
    int m_selectedIndex;
    int m_prevIndex;
    CMainWindow* mNewMainWindow;
    QPoint mOffset;
};

#endif // CONSOLETABBAR_H
