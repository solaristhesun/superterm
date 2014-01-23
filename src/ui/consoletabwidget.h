#ifndef CONSOLETABWIDGET_H
#define CONSOLETABWIDGET_H

#include <QTabWidget>

class ConsoleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConsoleTabWidget(QWidget *parent = 0);

signals:

public slots:
    void closeTab(int index);
};

#endif // CONSOLETABWIDGET_H
