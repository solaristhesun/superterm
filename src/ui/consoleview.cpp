#include <QScrollBar>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QDebug>
#include <QMouseEvent>

#include "consoleview.h"
#include "ui_consoleview.h"

#include "consoletab.h"

CConsoleView::CConsoleView(QWidget *parent) :
    QPlainTextEdit(parent),
    m_ui(new Ui::CConsoleView),
    m_parent(static_cast<CConsoleTab*>(parent)),
    m_bMouseDown(false),
    m_scrollBar(verticalScrollBar()),
    m_scrollPos(0),
    m_bAutoScroll(true)
{
    m_ui->setupUi(this);


    connect(m_scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarChanged(int)));
    setWordWrapMode(QTextOption::WrapAnywhere);
    refreshCursor();
}

CConsoleView::~CConsoleView()
{
    delete m_ui;
}

void CConsoleView::refreshCursor()
{
    const int iWidthSpace = fontMetrics().width(" ");
    setCursorWidth(iWidthSpace);
    setTabStopWidth(fontMetrics().width("01234567"));
}

void CConsoleView::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_pos = cursorForPosition(e->pos()).position();
        m_bMouseDown = true;
    }
    QPlainTextEdit::mousePressEvent(e);
}

void CConsoleView::mouseMoveEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
    {
        quint32 pos = cursorForPosition(e->pos()).position();

        QTextCursor cursor = textCursor();
        cursor.setPosition(m_pos, QTextCursor::MoveAnchor);
        cursor.setPosition(pos,QTextCursor::KeepAnchor);
        setTextCursor(cursor);
    }
    QPlainTextEdit::mouseMoveEvent(e);
}

void CConsoleView::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
    {
        copy();
        m_bMouseDown = false;
        insertPlainText(m_buffer);
        m_buffer = "";
    }
    QPlainTextEdit::mouseReleaseEvent(e);
}

void CConsoleView::keyPressEvent(QKeyEvent *e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        m_parent->toggleFullScreen();
        return;
    }

    emit keyPressed(e);
}

void CConsoleView::scrollDown(void)
{
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

void CConsoleView::scrollBarChanged(int pos)
{
    qDebug() << "SCROLL " << pos;
    m_scrollPos = pos;
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

void CConsoleView::saveCursor()
{
   this->cursorPos = this->textCursor().position();
   qDebug() << "saving position " << cursorPos;
}

void CConsoleView::restoreCursor()
{
   QTextCursor cursor = this->textCursor();
   qDebug() << "restoring position " << cursorPos;
   cursor.setPosition(this->cursorPos, QTextCursor::MoveAnchor);
   this->setTextCursor(cursor);
}

void CConsoleView::insertPlainText(const QString &text)
{
    if (m_bMouseDown)
    {
        m_buffer += text;
        return;
    }

    moveCursor(QTextCursor::End);
    QPlainTextEdit::insertPlainText(text);
    int iLines = text.count('\n');

    foreach (CHighlightsFrame::Highlighting h, m_highlightings)
    {
        for (int i = 0; i < iLines; i++)
        moveCursor(QTextCursor::Up);
        moveCursor(QTextCursor::StartOfLine);
        while (find(h.pattern))
        {
            //textCursor().select(QTextCursor::Document);
            QTextEdit::ExtraSelection extra;
            saveCursor();
            moveCursor(QTextCursor::Left); // workaround: why is this needed?
            extra.cursor = textCursor();
            qDebug() << "POS" << extra.cursor.position();
            extra.cursor.clearSelection();
            extra.format.setProperty(QTextFormat::FullWidthSelection, true);
            extra.format.setBackground( h.color );
            restoreCursor();
            m_extras << extra;

        }
    }

    setExtraSelections( m_extras );
    moveCursor(QTextCursor::End);

    if (!m_bAutoScroll)
    {
        m_scrollBar->setSliderPosition(m_scrollPos);
    }

}

void CConsoleView::insertBackspace()
{
    textCursor().deletePreviousChar();
}

void CConsoleView::insertNewline()
{
    textCursor().deletePreviousChar();
}

void CConsoleView::setAutoScroll(const bool bEnabled)
{
    m_bAutoScroll = bEnabled;

    if (m_bAutoScroll)
    {
        m_scrollBar->setSliderPosition(m_scrollBar->maximum());
    }
    else
    {
        m_scrollPos = m_scrollBar->maximum();
    }

}

// EOF <stefan@scheler.com>
