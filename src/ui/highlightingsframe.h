#ifndef HIGHLIGHTINGSFRAME_H
#define HIGHLIGHTINGSFRAME_H

#include <QColor>
#include <QFrame>

#include "models/highlighting.h"

namespace Ui
{
class HighlightingsFrame;
}

class QListWidgetItem;

class HighlightingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HighlightingsFrame(QWidget* parent = nullptr);
    ~HighlightingsFrame();

    QList<Highlighting> getItems();
    void                showEvent(QShowEvent* event);
    void                keyPressEvent(QKeyEvent* event);

public slots:
    void clear();
    void addHighlighting();
    void addHighlighting(QListWidgetItem* item);
    void deleteHighlighting();
    void onTextEdited(const QString& text);
    void onSelectionChanged();
    void changeColor();
    void deleteAll();

signals:
    void highlightingChanged();

private:
    Ui::HighlightingsFrame* m_ui;
    QColor                  m_color;

    void refreshColorButton();
};

#endif // HIGHLIGHTINGSFRAME_H
