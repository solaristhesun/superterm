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

#ifndef CONSOLELINE_H
#define CONSOLELINE_H

#include <QColor>
#include <QDateTime>
#include <QMetaType>

class ConsoleLine
{
public:
    ConsoleLine();

    void append(const QChar c);
    void setColor(const QColor& color);

    QDateTime timestamp() const;
    QColor    color() const;
    QString   text() const;

private:
    QDateTime timestamp_;
    QString   text_;
    QColor    color_;
};

Q_DECLARE_METATYPE(ConsoleLine)

#endif // CONSOLELINE_H
