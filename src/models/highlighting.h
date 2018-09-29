#ifndef HIGHLIGHTING_H
#define HIGHLIGHTING_H

#include <QColor>
#include <QMetaType>
#include <QString>

class Highlighting
{
public:
    QString pattern;
    QColor  color;
};

Q_DECLARE_METATYPE(Highlighting);

QDataStream& operator<<(QDataStream& out, const Highlighting& v);
QDataStream& operator>>(QDataStream& in, Highlighting& v);

#endif // HIGHLIGHTING_H
