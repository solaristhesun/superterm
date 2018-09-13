#include <QTimer>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "usagetracker.h"
#include "misc/uniqueidentifier.h"
#include "misc/globals.h"

UsageTracker::UsageTracker()
    : QObject(nullptr)
    , manager_(new QNetworkAccessManager(this))
{
    // currently empty
    connect(manager_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void UsageTracker::trackUsage()
{
    QSettings settings;
    QString uniqueId = settings.value("unique_id").toString();

    if (uniqueId.length() != 32)
    {
        UniqueIdentifier identifier;
        settings.setValue("unique_id", identifier.toString());
    }

    QNetworkRequest request;
    request.setUrl(QUrl("https://scheler.com/t/" + uniqueId));
    request.setRawHeader("User-Agent", getUserAgent());
    manager_->get(request);

    QTimer::singleShot(24*3600*1000, this, &UsageTracker::trackUsage);
}

void UsageTracker::replyFinished(QNetworkReply* reply)
{
    qDebug() << "RESPONSE" << reply->readAll();

    reply->deleteLater();
}

QByteArray UsageTracker::getUserAgent() const
{
    return QString(Globals::ApplicationFullName + QString(" (revision %1)").arg(Globals::ApplicationRevision)).toUtf8();
}

// EOF <stefan@scheler.com>
