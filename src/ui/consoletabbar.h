#ifndef CONSOLETABBAR_H
#define CONSOLETABBAR_H

#include <QTabBar>

class QPushButton;

class CConsoleTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CConsoleTabBar(QWidget *parent = 0);

    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent* event);
    void showEvent(QShowEvent *event);

    void	moveTab(int from, int to);


    void	tabInserted(int index);
    void tabRemoved(int index);
    void moveButton(void);

signals:
    void addButtonClicked(void);

public slots:

private:
    QPushButton *m_btn;
    int m_selectedIndex;
    int m_prevIndex;
};

#endif // CONSOLETABBAR_H
