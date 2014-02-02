#include <QApplication>
#include <QFile>

#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set application icon
    a.setWindowIcon(QIcon(":/icons/terminal_32x32.png"));

    // load styles from file
    QFile style(":/styles/superterm.qss");
    if(style.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(style.readAll());
        style.close();
    }

    // create main window
    MainWindow w;
    w.resize(800, 600);
    w.show();
    
    return a.exec();
}

// EOF <stefan@scheler.com>
