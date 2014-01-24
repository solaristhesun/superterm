#include <QScrollBar>
#include <iostream>
#include <QSerialPort>
#include <QKeyEvent>
#include <QMenu>
#include <QColorDialog>

#include "consoletab.h"
#include "ui_consoletab.h"

ConsoleTab::ConsoleTab(QSerialPort *port, QString title, QTabWidget *parent) :
    QTextEdit(parent),
    m_ui(new Ui::ConsoleTab),
    m_port(port),
    m_title(title),
    m_parent(parent),
    m_lastTabIndex(0)
{
    m_ui->setupUi(this);

    connect(m_port, SIGNAL(readyRead()), this, SLOT(showData()));
}

ConsoleTab::~ConsoleTab()
{
    delete m_port;
    delete m_ui;
}

void ConsoleTab::toggleFullScreen(void)
{
    if (!isFullScreen())
    {
        m_lastTabIndex = m_parent->currentIndex();
        setParent(0);
        showFullScreen();
    }
    else
    {
        setParent(m_parent);
        m_parent->insertTab(m_lastTabIndex, this, m_title);
        m_parent->setCurrentIndex(m_lastTabIndex);
    }
}

void ConsoleTab::keyPressEvent(QKeyEvent *e)
{
    if ((e->key()==Qt::Key_Return) && (e->modifiers()==Qt::AltModifier))
    {
        toggleFullScreen();
        return;
    }

    QByteArray data;
    data.append(e->text());

    m_port->write(data);
}

void ConsoleTab::showData()
{
    QByteArray data = m_port->readAll();
    for (int p = 0; p < data.size(); p++)
    {
        printf("0x%02x ", data.at(p));
    }
    printf("\n");

    QString str = data;
    std::cout << "NEW DATA [" << str.toStdString() << "]" << std::endl;
    str = str.replace("\r", "<br>");
    str = str.replace(" ", "&nbsp;");

    moveCursor(QTextCursor::End);
    textCursor().insertHtml(str);
    moveCursor(QTextCursor::End);

    scrollDown();
}

void ConsoleTab::showContextMenu(const QPoint &pt)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(m_ui->actionChangeColor);
    menu->exec(mapToGlobal(pt));
    delete menu;
}

void ConsoleTab::showColorDialog(void)
{
    QColor rgb = QColorDialog::getColor(Qt::white, this);
    setStyleSheet("QTextEdit { background-color: " + rgb.name() + " }");
}

void ConsoleTab::scrollDown(void)
{
    QScrollBar *sb = verticalScrollBar();
    sb->setValue(sb->maximum());
}

// EOF <stefan@scheler.com>
