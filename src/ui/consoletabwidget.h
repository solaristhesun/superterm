#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class CConsoleTab;
class CConsoleTabBar;

class CConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CConsoleTabWidget(QWidget *parent = 0);
    ~CConsoleTabWidget();

    void setCurrentTabText(const QString& test);
    void setConsoleFont(const QFont& font);

signals:

public slots:
    void closeTab(int index);
    void addNewTab(void);


private:
    CConsoleTabBar* m_tabBar;
};

#endif // CONSOLETABWIDGET_H
