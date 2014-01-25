#include <QDebug>
#include <QScrollBar>
#include <iostream>
#include <QKeyEvent>
#include <QMenu>
#include <QColorDialog>
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

    setCursorWidth(fontMetrics().width(' '));
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
    //m_port->write(data);
}

void ConsoleView::showContextMenu(const QPoint &pt)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(m_ui->actionChangeColor);
    menu->exec(mapToGlobal(pt));
    delete menu;
}

void ConsoleView::showColorDialog(void)
{
    QColor rgb = QColorDialog::getColor(palette().color(QPalette::Window), this);
    setStyleSheet(QString("QTextEdit { background-color: %1; }").arg(rgb.name()));
}

void ConsoleView::scrollDown(void)
{
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

// EOF <stefan@scheler.com>
