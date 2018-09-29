#ifndef CONSOLELINEBUFFER_H
#define CONSOLELINEBUFFER_H

#include <QAbstractListModel>
#include <QPair>
#include <QStringList>

#include "models/consoleline.h"

class ConsoleLineBuffer : public QAbstractListModel
{
public:
    ConsoleLineBuffer();

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void append(QByteArray data);
    void append(QString data);
    void clear();

    void addLineHighlighting(QString pattern, QColor color);

private:
    void appendToLastLine(QChar c);
    void createNewLine();
    void refreshHighlighting();
    void refreshSingleHighlighting(ConsoleLine& line);

private:
    QList<ConsoleLine>            list_;
    QList<QPair<QString, QColor>> highlighting_;
};

#endif // CONSOLELINEBUFFER_H
