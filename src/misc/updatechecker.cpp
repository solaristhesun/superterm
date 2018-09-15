#include <QTimer>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>

#include "misc/updatechecker.h"
#include "misc/uniqueidentifier.h"
#include "misc/globals.h"

UpdateChecker::UpdateChecker()
    : QObject(nullptr)
    , manager_(new QNetworkAccessManager(this))
{
    connect(manager_, &QNetworkAccessManager::finished,  this, &UpdateChecker::replyFinished);
}

void UpdateChecker::checkForUpdate()
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

    QTimer::singleShot(24*3600*1000, this, &UpdateChecker::checkForUpdate);
}

void UpdateChecker::replyFinished(QNetworkReply* reply)
{
    QString serverResponse = reply->readAll();

    QRegularExpression re("^([^ ]+) ([^ ]+)$"); // example response: "OK 2018.8a"
    QRegularExpressionMatch match = re.match(serverResponse);

    if (match.hasMatch())
    {
        QString opcode = match.captured(1);
        SoftwareVersion versionAvailable(match.captured(2));

        if (opcode == "OK" && versionAvailable.isValid())
        {
            if (versionAvailable > Globals::ApplicationVersion)
            {
                emit updateAvailable(versionAvailable);
            }
        }
    }

    reply->deleteLater();
}

QByteArray UpdateChecker::getUserAgent() const
{
    return QString(Globals::ApplicationFullName + QString(" (revision %1)").arg(Globals::ApplicationRevision)).toUtf8();
}

// EOF <stefan@scheler.com>
