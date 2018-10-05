#ifndef HIGHLIGHTINGSFRAME_H
#define HIGHLIGHTINGSFRAME_H

#include <QColor>
#include <QFrame>
#include <QItemSelection>

#include "models/highlighting.h"

namespace Ui
{
class HighlightingsFrame;
}

class HighlightingsModel;
class QSortFilterProxyModel;

class HighlightingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HighlightingsFrame(QWidget* parent = nullptr);
    virtual ~HighlightingsFrame() override;

    void showEvent(QShowEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void setModel(HighlightingsModel* model);

public slots:
    void addHighlighting();
    void deleteHighlighting();
    void onTextEdited(const QString& text);
    void onSelectionChanged();
    void changeColor();
    void deleteAll();
    void selectionChanged(const QItemSelection&, const QItemSelection&);

private:
    Ui::HighlightingsFrame* ui_;
    Highlighting            highlighting_;
    QSortFilterProxyModel*  proxyModel_;
    HighlightingsModel*     model_;

    void refreshColorButton();
};

#endif // HIGHLIGHTINGSFRAME_H
