#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>

#include "globals.h"
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

    QApplication::setOrganizationName("SCHELER");
    QApplication::setOrganizationDomain("scheler.com");
    QApplication::setApplicationName(g_sAppName);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load(g_sAppName + "_" + QLocale::system().name());
    a.installTranslator(&appTranslator);

    // create main window
    CMainWindow w;
    w.resize(800, 600);
    w.show();
    
    return a.exec();
}

// EOF <stefan@scheler.com>
