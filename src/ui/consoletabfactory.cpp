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

#include "ui/consoletabfactory.h"
#include "ui/consoletab.h"
#include "serial/portenumerator.h"
#include "session/session.h"

PortEnumerator* ConsoleTabFactory::sPortEnumerator = nullptr;

void ConsoleTabFactory::setPortEnumerator(PortEnumerator* portEnumerator)
{
    sPortEnumerator = portEnumerator;
}

ConsoleTab* ConsoleTabFactory::createTab()
{
    return new ConsoleTab(sPortEnumerator, nullptr);
}

ConsoleTab* ConsoleTabFactory::createTabFromSession(Session* session)
{
    ConsoleTab* tab = new ConsoleTab(sPortEnumerator, session);
    tab->setLabel(session->getTabLabel());

    return tab;
}

// EOF <stefan@scheler.com>
