#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QDir>

#include "session/session.h"

CSession::CSession()
    : m_bPortConnected(true)
{
    // currently nothing
}

CSession::~CSession()
{
    qDebug() << "CSession::~CSession";

    QString fileName = QCoreApplication::applicationDirPath() + "/superterm_" + m_deviceName + ".session";
    QFile::remove(fileName);
}

CSession* CSession::createSessionFromFile(const QString& fileName)
{
    QFile file(QCoreApplication::applicationDirPath() + "/" + fileName);

    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    CSession* session = new CSession();
    in >> *session;

    file.close();

    return session;
}

void CSession::saveToFile()
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

QStringList CSession::getSessionList()
{
    QDir dir(QCoreApplication::applicationDirPath());

    QStringList sessions = dir.entryList(QStringList() << "*.session", QDir::Files);

    return sessions;
}

void CSession::removeFiles()
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

void CSession::setPortConnected(bool bPortConnected)
{
    m_bPortConnected = bPortConnected;
}

bool CSession::isPortConnected() const
{
    return m_bPortConnected;
}

QDebug operator<<(QDebug dbg, const CSession& s)
{
    dbg.nospace() << "CSession(port=" << s.getDeviceName() << ")";

    return dbg.space();
}

// EOF <stefan@scheler.com>
