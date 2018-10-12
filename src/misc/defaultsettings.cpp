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

#include <QSettings>
#include <QColor>
#include <QFont>

#include "defaultsettings.h"

void DefaultSettings::writeDefaults()
{
    QSettings settings;

    if (!settings.contains("foreground"))
    {
        settings.setValue("foreground", QColor("white").name());
    }

    if (!settings.contains("background"))
    {
        settings.setValue("background", QColor("#142462").name());
    }

    if (!settings.contains("font"))
    {
#if defined(Q_OS_WIN)
        settings.setValue("font", QFont("Consolas", 11).toString());
#else
        settings.setValue("font", QFont("Liberation Mono", 11).toString());
#endif
    }
}

// EOF <stefan@scheler.com>
