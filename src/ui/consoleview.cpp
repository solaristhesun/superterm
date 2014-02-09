#include <QScrollBar>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QDebug>

#include "consoleview.h"
#include "ui_consoleview.h"

#include "consoletab.h"

CConsoleView::CConsoleView(QWidget *parent) :
    QPlainTextEdit(parent),
    m_ui(new Ui::CConsoleView),
    m_parent(static_cast<CConsoleTab*>(parent))
{
    m_ui->setupUi(this);

    setWordWrapMode(QTextOption::WrapAnywhere);
    refreshCursor();
}

CConsoleView::~CConsoleView()
{
    delete m_ui;
}

void CConsoleView::refreshCursor()
{
    setCursorWidth(fontMetrics().width(' ')-2);
}

void CConsoleView::keyPressEvent(QKeyEvent *e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        m_parent->toggleFullScreen();
        return;
    }

    QByteArray data;
    data.append(e->text());

    emit keyPressed(e->text());
}

void CConsoleView::scrollDown(void)
{
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

void CConsoleView::clear(void)
{
    m_extras.clear();
    QPlainTextEdit::clear();
}

void CConsoleView::setHighlighting(QList<CHighlightsFrame::Highlighting>& highlighting)
{
    m_highlightings = highlighting;
}

void CConsoleView::insertPlainText(const QString &text)
{
    moveCursor(QTextCursor::End);
    QPlainTextEdit::insertPlainText(text);

    foreach (CHighlightsFrame::Highlighting h, m_highlightings)
    {
        moveCursor(QTextCursor::Up);
        moveCursor(QTextCursor::StartOfLine);
        while (find(h.pattern))
        {
            textCursor().select(QTextCursor::Document);
            QTextEdit::ExtraSelection extra;
            extra.cursor = textCursor();
            extra.cursor.clearSelection();
            extra.format.setProperty(QTextFormat::FullWidthSelection, true);
            extra.format.setBackground( h.color );
            m_extras << extra;
        }
    }
    setExtraSelections( m_extras );
    moveCursor(QTextCursor::End);
}

// EOF <stefan@scheler.com>
