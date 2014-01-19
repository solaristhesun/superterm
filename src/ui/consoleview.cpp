#include <cstdio>

#include "consoleview.h"
#include "ui_consoleview.h"

ConsoleView::ConsoleView(QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::ConsoleView)
{
    ui->setupUi(this);
}

ConsoleView::~ConsoleView()
{
    delete ui;
}

void ConsoleView::keyPressEvent(QKeyEvent *)
{
    puts("FOO");
}
