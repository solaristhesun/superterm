/*
 * This file is part of superterm.
 *
 * Copyright (c) 2014-2018 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTimer>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>

#include "misc/updatechecker.h"
#include "misc/uniqueidentifier.h"
#include "misc/globals.h"
#include "misc/softwareversion.h"

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
