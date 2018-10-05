#include <QFile>
#include <QElapsedTimer>
#include <QModelIndex>
#include <QDebug>

#include "models/consolelinebuffer.h"
#include "models/highlightingsmodel.h"

ConsoleLineBuffer::ConsoleLineBuffer(HighlightingsModel* highlightingsModel)
    : QAbstractListModel()
    , highlightingsModel_(highlightingsModel)
    , logFile_(nullptr)
{
    clear();

    connect(highlightingsModel_, &HighlightingsModel::highlightingChanged, this, &ConsoleLineBuffer::refreshHighlighting);
}

ConsoleLineBuffer::~ConsoleLineBuffer()
{
    // currently empty
}

void ConsoleLineBuffer::appendToLastLine(QChar c)
{
    ConsoleLine& line = list_.last();

    line.append(c);

   // emit dataChanged(QAbstractListModel::index(list_.count()), QAbstractListModel::index(list_.count()));
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
    quint64 t;quint64 t2;
    int firstChangedRow = list_.count();
    for (int i = 0; i < data.size(); ++i)
    {
        const char c = data.at(i);
        appendToLastLine(c);

        if (c == '\n')
        {
            //timer2.start();
            refreshSingleHighlighting(list_.last());
            writeLineToLogFile(list_.last());
             timer2.start();
            //createNewLine();
                          timer2.start();
             beginInsertRows(QModelIndex(), list_.count(), list_.count());
t2 = timer2.nsecsElapsed();
             list_.append(ConsoleLine());
      //       qDebug () << "endInsertRows";
             endInsertRows();
                         t = timer2.nsecsElapsed();
        }
    }

    int lastChangedRow = list_.count();
    //qDebug() << "dataChanged" << firstChangedRow << lastChangedRow;
    emit dataChanged(QAbstractListModel::index(firstChangedRow), QAbstractListModel::index(lastChangedRow));

    //qDebug() << "append" << data.size() << timer.nsecsElapsed() << t2 <<t << list_.count();
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

    if (role == Qt::SizeHintRole)
    {
        const ConsoleLine& line = list_.at(index.row());
        return QVariant::fromValue(line.text().length());
    }

    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(list_.at(index.row()));
    }

    return QVariant();
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
    QColor color = highlightingsModel_->lineColor(line.text());

    if (color != line.color())
    {
        line.setColor(color);
        emit dataChanged(QAbstractListModel::index(list_.count()), QAbstractListModel::index(list_.count()));
    }
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
