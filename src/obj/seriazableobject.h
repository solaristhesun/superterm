#ifndef CSERIAZABLEOBJECT_H
#define CSERIAZABLEOBJECT_H

#include <QObject>
#include <QVariantList>

class CSeriazableObject : public QObject
{
    Q_OBJECT
public:
    explicit CSeriazableObject(QObject *parent = 0);

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

QDataStream &operator<<(QDataStream &ds, const CSeriazableObject &obj);
QDataStream &operator>>(QDataStream &ds, CSeriazableObject &obj) ;

#endif // CSERIAZABLEOBJECT_H
