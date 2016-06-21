#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QDebug>

#include "consoleviewex.h"

CConsoleViewEx::CConsoleViewEx(QWidget *parent)
    : QWidget(parent)
{
    // currently nothing
    resize(QSize(1000, 1000));
    setGeometry(0,0, 1000, 1000);
}

void CConsoleViewEx::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    p.setPen(QColor("white"));
    p.fillRect(rect(), QColor("#1b5464"));
    p.drawRect(rect());

    p.setPen(Qt::blue);
    p.setFont(QFont("Arial", 14));
    p.drawText(rect(), Qt::AlignCenter, "Qt");
    p.drawText(QPoint(20,20), "HALLO");
    p.drawText(QPoint(20,700), "HALLO");
    p.end();

    qDebug() << "RECT" << rect() << geometry();
}

// EOF <stefan@scheler.com>
