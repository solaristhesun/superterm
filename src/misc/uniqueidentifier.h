#ifndef UNIQUEIDENTIFIER_H
#define UNIQUEIDENTIFIER_H

#include <QString>

class QRandomGenerator;

class UniqueIdentifier
{
public:
    UniqueIdentifier();

    QString toString();

private:
    quint64 id1_;
    quint64 id2_;
};

#endif // UNIQUEIDENTIFIER_H
