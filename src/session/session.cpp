#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "session/session.h"

CSession::CSession()
{

}

void CSession::saveToFile()
{
    qDebug() << m_deviceName;

    QString fileName = QCoreApplication::applicationDirPath() + "/my" + m_deviceName + ".con";
    qDebug() << fileName;

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly))
    {
       qDebug() << file.errorString();
    }
    else
    {
        qDebug() << "Writing";
        QDataStream out(&file);
        out << *this;
    }

    file.close();
}
