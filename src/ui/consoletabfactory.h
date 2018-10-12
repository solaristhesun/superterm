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

#ifndef CONSOLETABFACTORY_H
#define CONSOLETABFACTORY_H

class PortEnumerator;
class ConsoleTab;
class Session;

class ConsoleTabFactory
{
public:
    static void        setPortEnumerator(PortEnumerator* portEnumerator);
    static ConsoleTab* createTab();
    static ConsoleTab* createTabFromSession(Session* session);

private:
    static PortEnumerator* sPortEnumerator;
};

#endif // CONSOLETABFACTORY_H
