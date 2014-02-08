#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QPlainTextEdit>

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
    void setHighlighting(QStringList& highlighting);

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
    QStringList                      m_highlightings;

    void saveCursor();
    void restoreCursor();

};

#endif // CONSOLEVIEW
