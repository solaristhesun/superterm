#include <QScrollBar>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QDebug>
#include <QMouseEvent>
#include <QTextBlock>

#include "ui_consoleview.h"
#include "ui/consoleview.h"
#include "ui/consoletab.h"

CConsoleView::CConsoleView(QWidget* parent)
    : QPlainTextEdit(parent),
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

void CConsoleView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_pos = cursorForPosition(e->pos()).position();
        m_bMouseDown = true;
    }
    QPlainTextEdit::mousePressEvent(e);
}

void CConsoleView::mouseMoveEvent(QMouseEvent* e)
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

void CConsoleView::mouseReleaseEvent(QMouseEvent* e)
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

void CConsoleView::keyPressEvent(QKeyEvent* e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        m_parent->toggleFullScreen();
        return;
    }

    emit keyPressed(e);
}

void CConsoleView::scrollDown()
{
    QScrollBar* sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

void CConsoleView::scrollBarChanged(int pos)
{
    qDebug() << "SCROLL " << pos;
    m_scrollPos = pos;
}

void CConsoleView::clear()
{
    m_extras.clear();
    QPlainTextEdit::clear();
}

void CConsoleView::setHighlighting(QList<CHighlightsFrame::Highlighting>& highlighting)
{
    m_highlightings = highlighting;
}

QList<CHighlightsFrame::Highlighting>& CConsoleView::getHighlighting()
{
    return m_highlightings;
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

void CConsoleView::insertPlainText(const QString& text)
{
    if (m_bMouseDown)
    {
        m_buffer += text;
        return;
    }

    wrapText(text, viewport()->width());

    int iLines = text.count('\n');

    foreach (CHighlightsFrame::Highlighting h, m_highlightings)
    {
        for (int i = 0; i < iLines; i++)
        {
            moveCursor(QTextCursor::Up);
        }
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

void CConsoleView::wrapText(QString textToWrap, const int width)
{
    QFontMetrics fm(font());

    int maxNrOfChars = width / fm.averageCharWidth();

    while (textToWrap.size() != 0)
    {
        int nrOfCharsInBlock = document()->lastBlock().length();
        int availableNrOfChars = qMax((maxNrOfChars - nrOfCharsInBlock), 0);

        QString elidedText = elideText(textToWrap, availableNrOfChars);

        moveCursor(QTextCursor::End);

        if (elidedText != textToWrap)
        {
            if (!elidedText.isEmpty())
            {
                QPlainTextEdit::insertPlainText(elidedText);
            }

            // only add a \n if string does not already contain one
            if (!elidedText.contains('\n'))
            {
                QPlainTextEdit::insertPlainText("\n");
            }
        }
        else
        {
            QPlainTextEdit::insertPlainText(textToWrap);
        }

        textToWrap.remove(0, elidedText.length());
    }
}

QString CConsoleView::elideText(const QString& text, int availableNrOfChars)
{
    QString elidedText = text.left(availableNrOfChars);

    /* if \n is inside of string (char 0 ... length-1) get its index
     * and add 1 to include it into the substring. this is important so that we
     * do not add an additional \n later on. if no \n is found, take the original
     * string. */
    int index = (elidedText.indexOf('\n') >= 0) ? elidedText.indexOf('\n')+1 : elidedText.length();
    elidedText = elidedText.left(index);

    return elidedText;
}

// EOF <stefan@scheler.com>
