#include <QFile>
#include <QElapsedTimer>
#include <QModelIndex>
#include <QDebug>

#include "consolelinebuffer.h"

ConsoleLineBuffer::ConsoleLineBuffer()
    : QAbstractListModel()
    , logFile_(nullptr)
{
    clear();
}

ConsoleLineBuffer::~ConsoleLineBuffer()
{
    // currently empty
}

void ConsoleLineBuffer::appendToLastLine(QChar c)
{
    ConsoleLine& line = list_.last();

    line.append(c);

    emit dataChanged(QAbstractListModel::index(list_.count()), QAbstractListModel::index(list_.count()));
}

void ConsoleLineBuffer::createNewLine()
{
    beginInsertRows(QModelIndex(), list_.count(), list_.count());
    list_.append(ConsoleLine());
    endInsertRows();
}

void ConsoleLineBuffer::clear()
{
    beginResetModel();
    list_.clear();
    createNewLine();
    endResetModel();
}

void ConsoleLineBuffer::append(QString data)
{
    ConsoleLineBuffer::append(data.toUtf8());
}

void ConsoleLineBuffer::append(QByteArray data)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer timer2;
    quint64 t;
    for (int i = 0; i < data.size(); ++i)
    {
        const char c = data.at(i);
        appendToLastLine(c);

        if (c == '\n')
        {
            timer2.start();
            refreshSingleHighlighting(list_.last());
            writeLineToLogFile(list_.last());
            createNewLine();
            t = timer2.nsecsElapsed();
        }
    }
    qDebug() << "append" << timer.nsecsElapsed() << t;
}

void ConsoleLineBuffer::writeLineToLogFile(ConsoleLine& line)
{
    if (logFile_)
    {
        QString lineStr = line.timestamp().toString("[yyyy-MM-dd HH:mm:ss.zzz] ") + line.text() + "\r\n";
        logFile_->write(lineStr.toUtf8());
        logFile_->flush();
    }
}

int ConsoleLineBuffer::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return list_.count();
}

QVariant ConsoleLineBuffer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= list_.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(list_.at(index.row()));
    }

    return QVariant();
}

void ConsoleLineBuffer::setHighlightings(QList<Highlighting> highlightings)
{
    highlightings_ = highlightings;
    refreshHighlighting();
}

void ConsoleLineBuffer::refreshHighlighting()
{
    for (int i = 0; i < list_.size(); ++i)
    {
        refreshSingleHighlighting(list_[i]);
    }
}

void ConsoleLineBuffer::refreshSingleHighlighting(ConsoleLine& line)
{
    for (int i = 0; i < highlightings_.size(); i++)
    {
        Highlighting highlighting = highlightings_.at(i);

        if (line.text().contains(highlighting.pattern))
        {
            line.setColor(highlighting.color);
            emit dataChanged(QAbstractListModel::index(list_.count()), QAbstractListModel::index(list_.count()));
            return;
        }
    }
    line.setColor(QColor::Invalid);
}

bool ConsoleLineBuffer::startLogging(QString fileName)
{
    if (logFile_ != nullptr)
    {
        stopLogging();
    }

    logFile_ = new QFile(fileName);

    if (!logFile_->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        return false;
    }

    return true;
}

void ConsoleLineBuffer::stopLogging()
{
    if (logFile_ != nullptr)
    {
        logFile_->close();
        delete logFile_;
        logFile_ = nullptr;
    }
}

// EOF <stefan@scheler.com>
