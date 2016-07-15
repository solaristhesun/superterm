#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QPlainTextEdit>

class QMouseEvent;
class QScrollBar;

#include "highlightsframe.h"

namespace Ui {
class CConsoleView;
}

class CConsoleTab;

class CConsoleView : public QPlainTextEdit
{
    Q_OBJECT

public:
    CConsoleView(QWidget* parent = 0);
    ~CConsoleView();

    void keyPressEvent(QKeyEvent* e);
    void scrollDown();
    void refreshCursor();
    void setHighlighting(QList<CHighlightsFrame::Highlighting>& highlighting);
    QList<CHighlightsFrame::Highlighting>& getHighlighting();
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

    void setAutoScroll(const bool bEnabled);
    void insertBackspace();
    void insertNewline();
    QSize getCharSize() const;

public slots:
    void clear();
    void insertPlainText(const QString& text);
    void scrollBarChanged(int pos);

signals:
    void keyPressed(QKeyEvent* e);

private:
    Ui::CConsoleView*                     m_ui;
    CConsoleTab*                          m_parent;
    QList<QTextEdit::ExtraSelection>      m_extras;
    int                                   m_cursorPos;
    QList<CHighlightsFrame::Highlighting> m_highlightings;
    bool                                  m_bMouseDown;
    QString                               m_buffer;
    quint32                               m_pos;
    QScrollBar*                           m_scrollBar;
    int                                   m_scrollPos;
    bool                                  m_bAutoScroll;
    int                                   cursorPos;

    void saveCursor();
    void restoreCursor();
    void wrapText(QString textToWrap, const int width);
    QString elideText(const QString& text, int availableNrOfChars);
};

#endif // CONSOLEVIEW
