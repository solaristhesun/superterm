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

#include <QRandomGenerator>

#include "misc/uniqueidentifier.h"

UniqueIdentifier::UniqueIdentifier()
    : id1_(0)
    , id2_(0)
{
    QRandomGenerator* random = QRandomGenerator::global();
    id1_ = random->generate64();
    id2_ = random->generate64();
}

QString UniqueIdentifier::toString()
{
    return QString("%1%2").arg(id1_, 16, 16, QLatin1Char('0')).arg(id2_, 16, 16, QLatin1Char('0'));
}

// EOF <stefan@scheler.com>
