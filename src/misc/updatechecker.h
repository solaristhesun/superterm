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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class SoftwareVersion;

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
