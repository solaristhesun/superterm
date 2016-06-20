#ifndef CCONNECTION_H
#define CCONNECTION_H

#include "obj/seriazableobject.h"

class QString;
class QDataStream;

class CConnection : public CSeriazableObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_name READ getName WRITE setName)

public:
    CConnection();

    QString getName() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

private:
    QString m_name;
};

#endif // CCONNECTION_H
