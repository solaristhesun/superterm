#include <QRandomGenerator>

#include "misc/uniqueidentifier.h"

UniqueIdentifier::UniqueIdentifier()
    : id1_(0)
    , id2_(0)
{
    QRandomGenerator* random = QRandomGenerator::global();
    id1_ = random->generate64();
    id2_ = random->generate64();
}

QString UniqueIdentifier::toString()
{
    return QString("%1%2").arg(id1_, 16, 16, QLatin1Char('0')).arg(id2_, 16, 16, QLatin1Char('0'));
}

// EOF <stefan@scheler.com>
