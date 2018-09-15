#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>

#include "misc/softwareversion.h"

class QNetworkAccessManager;
class QNetworkReply;

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    UpdateChecker();

public slots:
    void checkForUpdate();
    void replyFinished(QNetworkReply*);

signals:
    void updateAvailable(const SoftwareVersion& version);

private:
    QNetworkAccessManager* manager_;

private:
    QByteArray getUserAgent() const;
};

#endif // UPDATECHECKER_H
