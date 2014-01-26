#include <QApplication>

#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/terminal_16x16.png"));

    MainWindow w;
    w.show();
    
    return a.exec();
}

// EOF <stefan@schelre.com>
