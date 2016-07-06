#ifndef SERIALIZABLEOBJECT_H
#define SERIALIZABLEOBJECT_H

#include <QObject>
#include <QVariantList>

class CSerializableObject : public QObject
{
    Q_OBJECT
public:
    explicit CSerializableObject(QObject *parent = 0);

    template<typename T>
    static QVariantList convertToQVariantList(QList<T> types)
    {
        QVariantList varList;

        foreach(const T& type, types)
        {
            varList.append(QVariant::fromValue(type));
        }

        return varList;
    }

signals:

public slots:
};

QDataStream &operator<<(QDataStream &ds, const CSerializableObject &obj);
QDataStream &operator>>(QDataStream &ds, CSerializableObject &obj) ;

#endif // SERIALIZABLEOBJECT_H
