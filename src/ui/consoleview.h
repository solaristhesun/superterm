#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QPlainTextEdit>

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

public slots:
    void clear();
    void insertPlainText(const QString &text);

signals:
    void keyPressed(QString text);

private:
    Ui::CConsoleView*                 m_ui;
    CConsoleTab*                      m_parent;
    QList<QTextEdit::ExtraSelection> m_extras;
    int                              m_cursorPos;
    QList<CHighlightsFrame::Highlighting>                      m_highlightings;
};

#endif // CONSOLEVIEW
