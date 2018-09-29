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

signals:

public slots:
};

QDataStream& operator<<(QDataStream& ds, const SerializableObject& obj);
QDataStream& operator>>(QDataStream& ds, SerializableObject& obj);

#endif // SERIALIZABLEOBJECT_H
