#include <QMetaProperty>
#include <QVariant>

#include "seriazableobject.h"

CSeriazableObject::CSeriazableObject(QObject *parent) : QObject(parent)
{

}

QDataStream &operator<<(QDataStream &ds, const CSeriazableObject &obj) {
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
        if(obj.metaObject()->property(i).isStored(&obj)) {
            QVariant var = obj.metaObject()->property(i).read(&obj);
            ds << var;
        }
    }
    return ds;
}

QDataStream &operator>>(QDataStream &ds, CSeriazableObject &obj) {
    QVariant var;
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
        if(obj.metaObject()->property(i).isStored(&obj)) {
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }
    return ds;
}
