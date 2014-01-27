#include <QScrollBar>
#include <QKeyEvent>
#include <QFontMetrics>

#include "consoleview.h"
#include "ui_consoleview.h"

#include "consoletab.h"

ConsoleView::ConsoleView(QWidget *parent) :
    QTextEdit(parent),
    m_ui(new Ui::ConsoleView),
    m_parent(static_cast<ConsoleTab*>(parent))
{
    m_ui->setupUi(this);

    setCursorWidth(fontMetrics().width(' ')-2);
}

ConsoleView::~ConsoleView()
{
    delete m_ui;
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

// EOF <stefan@scheler.com>
