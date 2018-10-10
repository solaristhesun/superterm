#ifndef CONSOLELINEBUFFER_H
#define CONSOLELINEBUFFER_H

#include <QAbstractListModel>
#include <QPair>
#include <QStringList>

#include "models/consoleline.h"

class QFile;
class HighlightingsModel;

class ConsoleLineBuffer : public QAbstractListModel
{
    Q_OBJECT
public:
    ConsoleLineBuffer(HighlightingsModel* highlightingsModel);
    virtual ~ConsoleLineBuffer() override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool startLogging(QString fileName);
    void stopLogging();
    bool isLogging() const;

    void append(QByteArray data);
    void append(QString data);
    void clear();

private slots:
    void refreshHighlighting();
    void completeInsertion();

private:
    void appendToLastLine(QChar c);
    void createNewLine();
    void refreshSingleHighlighting(ConsoleLine& line);
    void writeLineToLogFile(ConsoleLine& line);

private:
    QVector<ConsoleLine> list_;
    HighlightingsModel*  highlightingsModel_;
    QFile*               logFile_;
    bool                 bInsertStarted_;
    int                  firstRow_;
    int                  lastRow_;
};

#endif // CONSOLELINEBUFFER_H
