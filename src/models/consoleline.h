#ifndef CONSOLELINE_H
#define CONSOLELINE_H

#include <QColor>
#include <QDateTime>
#include <QMetaType>

class ConsoleLine
{
public:
    ConsoleLine();

    void      append(QChar c);
    void      setColor(QColor color);

    QDateTime timestamp();
    QColor    color();
    QString   text();

private:
    QDateTime timestamp_;
    QString   text_;
    QColor    color_;
};

Q_DECLARE_METATYPE(ConsoleLine)

#endif // CONSOLELINE_H
