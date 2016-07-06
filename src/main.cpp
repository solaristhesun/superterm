#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDebug>

#include "globals.h"
#include "ui/mainwindow.h"
#include "port/portapplication.h"
#include "enumerator/portenumerator.h"
#include "ui/consoletabfactory.h"

int main(int argc, char *argv[])
{
    if (argc == 7)
    {
        CPortApplication a(argc, argv);
        a.connectSocket();

        return a.exec();
    }
    else
    {
        QApplication a(argc, argv);

        // set application icon
        a.setWindowIcon(QIcon(":/icons/terminal_32x32.png"));

        // load styles from file
        QFile style(":/styles/superterm.qss");

        if (style.open(QIODevice::ReadOnly | QIODevice::Text))
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
        appTranslator.load(QCoreApplication::applicationDirPath() + "/" + g_sAppName + "_" + QLocale::system().name());
        a.installTranslator(&appTranslator);

        CPortEnumerator portEnumerator;
        CConsoleTabFactory::setPortEnumerator(&portEnumerator);

        // create main window on heap!
        CMainWindow* w = new CMainWindow;
        w->resize(800, 600);
        w->addExistingTabsFromFile();
        w->show();

        return a.exec();
    }
}

// EOF <stefan@scheler.com>
