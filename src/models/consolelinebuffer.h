#ifndef CONSOLELINEBUFFER_H
#define CONSOLELINEBUFFER_H

#include <QAbstractListModel>
#include <QPair>
#include <QStringList>

#include "models/consoleline.h"
#include "models/highlighting.h"

class QFile;

class ConsoleLineBuffer : public QAbstractListModel
{
    Q_OBJECT
public:
    ConsoleLineBuffer();
    virtual ~ConsoleLineBuffer() override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool startLogging(QString fileName);
    void stopLogging();

    void append(QByteArray data);
    void append(QString data);
    void clear();
    void setHighlightings(QList<Highlighting> highlightings);

private:
    void appendToLastLine(QChar c);
    void createNewLine();
    void refreshHighlighting();
    void refreshSingleHighlighting(ConsoleLine& line);
    void writeLineToLogFile(ConsoleLine& line);

private:
    QVector<ConsoleLine>  list_;
    QList<Highlighting> highlightings_;
    QFile*              logFile_;
};

#endif // CONSOLELINEBUFFER_H
