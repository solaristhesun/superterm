#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class ConsoleTab;

class ConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConsoleTabWidget(QWidget *parent = 0);

    void addTab(ConsoleTab *tab);
    void setCurrentTabText(const QString& test);

signals:

public slots:
    void closeTab(int index);

private:
    QSize m_origButtonSize;
};

#endif // CONSOLETABWIDGET_H
