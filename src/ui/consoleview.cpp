#include <QScrollBar>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QDebug>

#include "consoleview.h"
#include "ui_consoleview.h"

#include "consoletab.h"

ConsoleView::ConsoleView(QWidget *parent) :
    QPlainTextEdit(parent),
    m_ui(new Ui::ConsoleView),
    m_parent(static_cast<ConsoleTab*>(parent))
{
    m_ui->setupUi(this);

    setWordWrapMode(QTextOption::WrapAnywhere);
    refreshCursor();
}

ConsoleView::~ConsoleView()
{
    delete m_ui;
}

void ConsoleView::refreshCursor()
{
    setCursorWidth(fontMetrics().width(' ')-2);
}

void ConsoleView::keyPressEvent(QKeyEvent *e)
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

void ConsoleView::scrollDown(void)
{
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ConsoleView::highlight(void)
{
    QList<QTextEdit::ExtraSelection> m_extras;
    moveCursor(QTextCursor::Start);
    while (find("[UI_STATE]" ))
    {
        textCursor().select(QTextCursor::Document);
        qDebug() << textCursor().selectedText();
        QTextEdit::ExtraSelection extra;
        extra.cursor = textCursor();
        extra.cursor.clearSelection();
        extra.format.setProperty(QTextFormat::FullWidthSelection, true);
        extra.format.setBackground( Qt::red );
        m_extras << extra;
    }

    setExtraSelections( m_extras );
    moveCursor(QTextCursor::End);
}

// EOF <stefan@scheler.com>
