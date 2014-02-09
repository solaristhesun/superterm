#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QPlainTextEdit>

#include "highlightsframe.h"

namespace Ui {
    class ConsoleView;
}

class ConsoleTab;

class ConsoleView : public QPlainTextEdit
{
    Q_OBJECT
    
public:
    ConsoleView(QWidget *parent = 0);
    ~ConsoleView();

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
    Ui::ConsoleView*                 m_ui;
    ConsoleTab*                      m_parent;
    QList<QTextEdit::ExtraSelection> m_extras;
    int                              m_cursorPos;
    QList<CHighlightsFrame::Highlighting>                      m_highlightings;
};

#endif // CONSOLEVIEW
