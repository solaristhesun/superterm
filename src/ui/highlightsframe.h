#ifndef HIGHLIGHTSFRAME_H
#define HIGHLIGHTSFRAME_H

#include <QFrame>
#include <QColor>

namespace Ui {
class CHighlightsFrame;
}

class QListWidgetItem;

class CHighlightsFrame : public QFrame
{
    Q_OBJECT

public:
    struct Highlighting
    {
        QString pattern;
        QColor color;
    };

    explicit CHighlightsFrame(QWidget *parent = 0);
    ~CHighlightsFrame();

    QList<Highlighting> getItems(void);
    void showEvent(QShowEvent * event);
    void keyPressEvent ( QKeyEvent * event );

public slots:
    void clear(void);
    void addHighlighting(void);
    void addHighlighting(QListWidgetItem *item);
    void deleteHighlighting(void);
    void onTextEdited(const QString& text);
    void onSelectionChanged();
    void changeColor(void);
    void deleteAll(void);

signals:
    void highlightingChanged(void);

private:
    Ui::CHighlightsFrame* m_ui;
    QColor                m_color;

    void refreshColorButton();
};

Q_DECLARE_METATYPE(CHighlightsFrame::Highlighting)

#endif // HIGHLIGHTSFRAME_H
