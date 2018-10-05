#ifndef CONSOLELINE_H
#define CONSOLELINE_H

#include <QColor>
#include <QDateTime>
#include <QMetaType>

class ConsoleLine
{
public:
    ConsoleLine();

    void append(QChar c);
    void setColor(const QColor& color);

    QDateTime timestamp();
    QColor    color() const;
    QString   text() const;

private:
    QDateTime timestamp_;
    QString   text_;
    QColor    color_;
};

Q_DECLARE_METATYPE(ConsoleLine)

#endif // CONSOLELINE_H
