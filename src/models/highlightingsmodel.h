#ifndef HIGHLIGHTINGSMODEL_H
#define HIGHLIGHTINGSMODEL_H

#include <QAbstractListModel>

#include "models/highlighting.h"

class HighlightingsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HighlightingsModel(QObject* parent = nullptr);

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void                setHighlightings(QList<Highlighting> highlightings);
    void                addHighlighting(Highlighting highlighting);
    QList<Highlighting> highlightings() const;
    void                remove(const QModelIndex& index);
    void                removeAll();
    QColor              lineColor(QString line);

signals:
    void highlightingChanged();

private:
    QList<Highlighting> highlightings_;
};

#endif // HIGHLIGHTINGSMODEL_H
