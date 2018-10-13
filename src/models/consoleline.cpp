/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "consoleline.h"

ConsoleLine::ConsoleLine()
    : timestamp_()
{
    // currently empty
}

void ConsoleLine::append(const QChar c)
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

QDateTime ConsoleLine::timestamp() const
{
    return timestamp_;
}

QColor ConsoleLine::color() const
{
    return color_;
}

void ConsoleLine::setColor(const QColor& color)
{
    color_ = color;
}

QString ConsoleLine::text() const
{
    return text_;
}

// EOF <stefan@scheler.com>
