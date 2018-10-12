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

#include <QMetaProperty>
#include <QVariant>

#include "session/serializableobject.h"

SerializableObject::SerializableObject(QObject* parent)
    : QObject(parent)
{
    // currently nothing
}

QDataStream& operator<<(QDataStream& ds, const SerializableObject& obj)
{
    for (int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if (obj.metaObject()->property(i).isStored(&obj))
        {
            QVariant var = obj.metaObject()->property(i).read(&obj);
            ds << var;
        }
    }
    return ds;
}

QDataStream& operator>>(QDataStream& ds, SerializableObject& obj)
{
    QVariant var;

    for (int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if (obj.metaObject()->property(i).isStored(&obj))
        {
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }

    return ds;
}
