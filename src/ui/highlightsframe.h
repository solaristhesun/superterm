#ifndef HIGHLIGHTSFRAME_H
#define HIGHLIGHTSFRAME_H

#include <QFrame>
#include <QColor>

namespace Ui {
class CHighlightsFrame;
}

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

public slots:
    void addHighlighting(void);
    void deleteHighlighting(void);
    void onTextEdited(const QString& text);
    void onSelectionChanged();
    void changeColor(void);

signals:
    void highlightingChanged(void);

private:
    Ui::CHighlightsFrame* m_ui;
    QColor                m_color;
};

#endif // HIGHLIGHTSFRAME_H
