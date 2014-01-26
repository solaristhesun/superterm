#ifndef CONSOLETABBAR_H
#define CONSOLETABBAR_H

#include <QTabBar>

class ConsoleTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit ConsoleTabBar(QWidget *parent = 0);

    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent* event);
    void showEvent(QShowEvent *event);

signals:
    void addButtonClicked(void);

public slots:

private:
    int m_selectedIndex;
    int m_prevIndex;

    int lastTabIndex(void);

};

#endif // CONSOLETABBAR_H
