#ifndef CONSOLELINEBUFFER_H
#define CONSOLELINEBUFFER_H

#include <QAbstractListModel>
#include <QPair>
#include <QStringList>

#include "models/consoleline.h"
#include "models/highlighting.h"

class ConsoleLineBuffer : public QAbstractListModel
{
    Q_OBJECT
public:
    ConsoleLineBuffer();
    virtual ~ConsoleLineBuffer() override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void append(QByteArray data);
    void append(QString data);
    void clear();

    void setHighlightings(QList<Highlighting> highlightings);

private:
    void appendToLastLine(QChar c);
    void createNewLine();
    void refreshHighlighting();
    void refreshSingleHighlighting(ConsoleLine& line);

private:
    QList<ConsoleLine>  list_;
    QList<Highlighting> highlightings_;
};

#endif // CONSOLELINEBUFFER_H
