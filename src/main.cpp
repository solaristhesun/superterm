#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDebug>

#include "misc/globals.h"
#include "ui/mainwindow.h"
#include "ui/consoletabfactory.h"
#include "serial/portapplication.h"
#include "serial/portenumerator.h"
#include "singleapplication/singleapplication.h"

int main(int argc, char* argv[])
{
    if (argc == 7)
    {
        CPortApplication a(argc, argv);
        return a.exec();
    }
    else
    {
        QApplication::setOrganizationName("SCHELER");
        QApplication::setOrganizationDomain("scheler.com");
        QApplication::setApplicationName(g_sAppName);

        SingleApplication a(argc, argv);

        // set application icon
        a.setWindowIcon(QIcon(":/icons/terminal_32x32.png"));

        // load styles from file
        QFile style(":/styles/superterm.qss");

        if (style.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            a.setStyleSheet(style.readAll());
            style.close();
        }

        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&qtTranslator);

        QTranslator appTranslator;
        appTranslator.load(QCoreApplication::applicationDirPath() + "/" + g_sAppName + "_" + QLocale::system().name());
        a.installTranslator(&appTranslator);

        CPortEnumerator portEnumerator;
        CConsoleTabFactory::setPortEnumerator(&portEnumerator);

        // create main window on heap!
        CMainWindow* w = new CMainWindow;
        w->addExistingTabsFromFile();
        w->show();

        QObject::connect(&a, &QApplication::aboutToQuit, w, &CMainWindow::deleteLater);
        QObject::connect(&a, &SingleApplication::showUp, w, &CMainWindow::onSecondaryInstanceLaunched);

        return a.exec();
    }
}

// EOF <stefan@scheler.com>
