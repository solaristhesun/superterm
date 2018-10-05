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
