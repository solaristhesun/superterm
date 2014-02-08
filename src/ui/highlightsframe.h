#ifndef HIGHLIGHTSFRAME_H
#define HIGHLIGHTSFRAME_H

#include <QFrame>

namespace Ui {
class CHighlightsFrame;
}

class CHighlightsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CHighlightsFrame(QWidget *parent = 0);
    ~CHighlightsFrame();

    QStringList getItems(void);
    void showEvent(QShowEvent * event);

public slots:
    void addHighlighting(void);
    void deleteHighlighting(void);
    void hideFrame(void);
    void onTextEdited(const QString& text);
    void onSelectionChanged();

signals:
    void closed(void);

private:
    Ui::CHighlightsFrame *m_ui;
};

#endif // HIGHLIGHTSFRAME_H
