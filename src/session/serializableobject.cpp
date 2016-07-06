#include <QMetaProperty>
#include <QVariant>

#include "session/serializableobject.h"

CSerializableObject::CSerializableObject(QObject *parent)
    : QObject(parent)
{
    // currently nothing
}

QDataStream &operator<<(QDataStream &ds, const CSerializableObject &obj)
{
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if(obj.metaObject()->property(i).isStored(&obj))
        {
            QVariant var = obj.metaObject()->property(i).read(&obj);
            ds << var;
        }
    }
    return ds;
}

QDataStream &operator>>(QDataStream &ds, CSerializableObject &obj)
{
    QVariant var;

    for(int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if(obj.metaObject()->property(i).isStored(&obj))
        {
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }

    return ds;
}
