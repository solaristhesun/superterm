#include "consoleline.h"

ConsoleLine::ConsoleLine()
    : timestamp_()
{
    // currently empty
}

void ConsoleLine::append(QChar c)
{
    if (text_.length() == 0)
    {
        timestamp_ = QDateTime::currentDateTime();
    }

    switch (c.toLatin1())
    {
        case 0x08:
            text_.chop(1);
            break;
        case '\r':
        case '\n':
            break;
        default:
            text_.append(c);
            break;
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
