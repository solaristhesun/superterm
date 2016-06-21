#ifndef CCONSOLEVIEWEX_H
#define CCONSOLEVIEWEX_H

#include <QWidget>

class CConsoleViewEx : public QWidget
{
    Q_OBJECT
public:
    explicit CConsoleViewEx(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // CCONSOLEVIEWEX_H
