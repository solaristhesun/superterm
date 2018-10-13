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

#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <QObject>
#include <QVariantList>

class SerializableObject : public QObject
{
    Q_OBJECT
public:
    explicit SerializableObject(QObject* parent = nullptr);

    template <typename T>
    static QVariantList convertToQVariantList(QList<T> types)
    {
        QVariantList varList;

        for (const T& type : types) {
            varList.append(QVariant::fromValue(type));
        }

        return varList;
    }
};

QDataStream& operator<<(QDataStream& ds, const SerializableObject& obj);
QDataStream& operator>>(QDataStream& ds, SerializableObject& obj);

#endif // SERIALIZABLEOBJECT_H
