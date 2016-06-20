#ifndef CSERIAZABLEOBJECT_H
#define CSERIAZABLEOBJECT_H

#include <QObject>

class CSeriazableObject : public QObject
{
    Q_OBJECT
public:
    explicit CSeriazableObject(QObject *parent = 0);

signals:

public slots:
};

QDataStream &operator<<(QDataStream &ds, const CSeriazableObject &obj);
QDataStream &operator>>(QDataStream &ds, CSeriazableObject &obj) ;

#endif // CSERIAZABLEOBJECT_H
