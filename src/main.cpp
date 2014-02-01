#include <QApplication>
#include <QDebug>

#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/terminal_32x32.png"));

    MainWindow w;
    w.resize(800, 600);
    w.show();
    
    return a.exec();
}

// EOF <stefan@scheler.com>
