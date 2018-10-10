#include <QFile>
#include <QTimer>
#include <QElapsedTimer>
#include <QModelIndex>
#include <QDebug>

#include "models/consolelinebuffer.h"
#include "models/highlightingsmodel.h"

QElapsedTimer timer;

ConsoleLineBuffer::ConsoleLineBuffer(HighlightingsModel* highlightingsModel)
    : QAbstractListModel()
    , highlightingsModel_(highlightingsModel)
    , logFile_(nullptr)
    , bInsertStarted_(false)
{
    clear();
    timer.start();

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
    const int firstChangedRow = list_.count();
    bool bLineCompleted = false;

    qDebug() << "append" << data;

    int currentLines = list_.count();
    int numLines = data.count('\n');

    //qDebug() << "append" << data << currentLines << numLines;

    if (numLines > 0)
    {
        //qDebug() << "beginInsertRows" << currentLines <<  currentLines + numLines - 1;
        beginInsertRows(QModelIndex(), currentLines, currentLines + numLines -1);
    }

    for (int i = 0; i < data.size(); ++i)
    {
        const char c = data.at(i);
        appendToLastLine(c);

        if (c == '\n')
        {
            bLineCompleted = true;
            refreshSingleHighlighting(list_.last());
            writeLineToLogFile(list_.last());
            list_.append(ConsoleLine());
        }
    }

    if (numLines > 0)
    {
        endInsertRows();
    }

    const int lastChangedRow = list_.count();

    if (!bLineCompleted)
    {
        QModelIndex index = createIndex(list_.count(), 0);
        //qDebug() << "data changed";
        emit dataChanged(index, index);
    }

    //qDebug() << "append" << timer.elapsed() << data.size() << data;
}

void ConsoleLineBuffer::completeInsertion()
{
    qDebug() << "completeInsertion" << firstRow_ << lastRow_;
    beginInsertRows(QModelIndex(), firstRow_, lastRow_);
    endInsertRows();
    bInsertStarted_ = false;
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

    if (role == Qt::DisplayRole)
    {
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

bool ConsoleLineBuffer::isLogging() const
{
    return logFile_ != nullptr;
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
