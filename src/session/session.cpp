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

#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QDir>

#include "session/session.h"

Session::Session()
    : bPortConnected_(true)
{
    // currently nothing
}

Session::~Session()
{
    qDebug() << "CSession::~CSession";

    QString fileName = QCoreApplication::applicationDirPath() + "/superterm_" + m_deviceName + ".session";
    QFile::remove(fileName);
}

Session* Session::createSessionFromFile(const QString& fileName)
{
    QFile file(QCoreApplication::applicationDirPath() + "/" + fileName);

    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    Session* session = new Session();
    in >> *session;

    file.close();

    return session;
}

void Session::saveToFile()
{
    QString fileName = QCoreApplication::applicationDirPath() + "/superterm_" + m_deviceName + ".session";
    QFile   file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.errorString();
    }
    else
    {
        qDebug() << "writing session" << fileName;

        QDataStream out(&file);
        out << *this;
    }

    file.close();
}

QStringList Session::getSessionList()
{
    QDir dir(QCoreApplication::applicationDirPath());

    QStringList sessions = dir.entryList(QStringList() << "*.session", QDir::Files);

    return sessions;
}

void Session::removeFiles()
{
    QDir        dir(QCoreApplication::applicationDirPath());
    QStringList files = dir.entryList(QStringList() << "*.session", QDir::Files);

    if (!files.isEmpty())
    {
        for (const QString& file : files)
        {
            dir.remove(file);
        }
    }
}

void Session::setPortConnected(bool bPortConnected)
{
    bPortConnected_ = bPortConnected;
}

bool Session::isPortConnected() const
{
    return bPortConnected_;
}

QDebug operator<<(QDebug dbg, const Session& s)
{
    dbg.nospace() << "CSession(port=" << s.getDeviceName() << ")";

    return dbg.space();
}

// EOF <stefan@scheler.com>
