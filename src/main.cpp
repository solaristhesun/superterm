#include <QApplication>

#include "ui/mainwindow.h"
#include "consolemgr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConsoleMgr consoleMgr;

    MainWindow w(consoleMgr);
    w.show();
    
    return a.exec();
}
