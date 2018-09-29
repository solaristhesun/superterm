#ifndef HIGHLIGHTINGSFRAME_H
#define HIGHLIGHTINGSFRAME_H

#include <QColor>
#include <QFrame>

#include "models/highlighting.h"

namespace Ui
{
class CHighlightingsFrame;
}

class QListWidgetItem;

class CHighlightingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CHighlightingsFrame(QWidget* parent = nullptr);
    ~CHighlightingsFrame();

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
    Ui::CHighlightingsFrame* m_ui;
    QColor                   m_color;

    void refreshColorButton();
};

#endif // HIGHLIGHTINGSFRAME_H
