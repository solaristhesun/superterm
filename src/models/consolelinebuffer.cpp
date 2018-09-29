#include <QModelIndex>
#include <QDebug>

#include "consolelinebuffer.h"

ConsoleLineBuffer::ConsoleLineBuffer()
    : QAbstractListModel()
{
    clear();
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
    for (int i = 0; i < data.size(); ++i)
    {
        const char c = data.at(i);
        appendToLastLine(c);

        if (c == '\n')
        {
            refreshSingleHighlighting(list_.last());
            createNewLine();
        }
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

void ConsoleLineBuffer::addLineHighlighting(QString pattern, QColor color)
{
    QPair<QString, QColor> highlighting;
    highlighting.first = pattern;
    highlighting.second = color;
    highlighting_.append(highlighting);
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
    for (int i = 0; i < highlighting_.size(); i++)
    {
        QPair<QString, QColor> highlight;

        if (line.text().contains(highlight.first))
        {
            //qDebug() << "MATCH" << line.text() << highlight.first;
            line.setColor(highlight.second);
            emit dataChanged(QAbstractListModel::index(list_.count()), QAbstractListModel::index(list_.count()));
            break;
        }
    }
}

// EOF <stefan@scheler.com>
