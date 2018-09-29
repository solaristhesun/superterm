#ifndef CCLICKABLELABEL_H
#define CCLICKABLELABEL_H

#include <QLabel>

class CClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CClickableLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CCLICKABLELABEL_H
