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
    CConsoleView(QWidget *parent = 0);
    ~CConsoleView();

    void keyPressEvent(QKeyEvent * e);
    void scrollDown(void);
    void refreshCursor();
    void setHighlighting(QList<CHighlightsFrame::Highlighting>& highlighting);
    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);

    void setAutoScroll(const bool bEnabled);

public slots:
    void clear();
    void insertPlainText(const QString &text);
    void scrollBarChanged(int pos);


signals:
    void keyPressed(QString text);

private:
    Ui::CConsoleView*                 m_ui;
    CConsoleTab*                      m_parent;
    QList<QTextEdit::ExtraSelection> m_extras;
    int                              m_cursorPos;
    QList<CHighlightsFrame::Highlighting>                      m_highlightings;
    bool                              m_bMouseDown;
    QString                           m_buffer;
    quint32                           m_pos;
    QScrollBar*                       m_scrollBar;
    int                               m_scrollPos;
    bool                              m_bAutoScroll;
    int cursorPos;

    void saveCursor(void);
    void restoreCursor(void);
};

#endif // CONSOLEVIEW
