#include "consoleline.h"

ConsoleLine::ConsoleLine()
    : timestamp_(QDateTime::currentDateTime())
{
    // currently empty
}

void ConsoleLine::append(QChar c)
{
    if (text_.length() == 0)
    {
        timestamp_ = QDateTime::currentDateTime();
    }

    if (c == 0x08)
    {
        text_.chop(1);
    }
    else
    {
        text_.append(c);
    }
}

QDateTime ConsoleLine::timestamp()
{
    return timestamp_;
}

QColor ConsoleLine::color()
{
    return color_;
}

void ConsoleLine::setColor(QColor color)
{
    color_ = color;
}

QString ConsoleLine::text()
{
    return text_;
}

// EOF <stefan@scheler.com>
