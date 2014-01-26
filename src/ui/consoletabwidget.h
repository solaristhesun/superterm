#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class ConsoleTab;
class ConsoleTabBar;

class ConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConsoleTabWidget(QWidget *parent = 0);
    ~ConsoleTabWidget();

    void addTab(ConsoleTab *tab);
    void setCurrentTabText(const QString& test);

signals:

public slots:
    void onCloseTab(int index);
    void onAddButtonClicked(void);


private:
    ConsoleTabBar* m_tabBar;
};

#endif // CONSOLETABWIDGET_H
