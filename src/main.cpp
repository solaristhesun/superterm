#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDebug>

#include "misc/globals.h"
#include "misc/defaultsettings.h"
#include "misc/updatechecker.h"
#include "ui/mainwindow.h"
#include "ui/consoletabfactory.h"
#include "ui/highlightingsframe.h"
#include "serial/portapplication.h"
#include "serial/portenumerator.h"
#include "singleapplication/singleapplication.h"
#include "models/highlighting.h"
#include "misc/softwareversion.h"

int main(int argc, char* argv[])
{
    if (argc == 7)
    {
        PortApplication a(argc, argv);
        return a.exec();
    }
    else
    {
        QApplication::setOrganizationName("SCHELER");
        QApplication::setOrganizationDomain("scheler.com");
        QApplication::setApplicationName(Globals::ApplicationName);

        SingleApplication a(argc, argv);

        QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);

        // set application icon
        a.setWindowIcon(QIcon(":/icons/terminal_32x32.png"));

        // load styles from file
        QFile style(":/styles/superterm.qss");

        if (style.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            a.setStyleSheet(style.readAll());
            style.close();
        }

        DefaultSettings::writeDefaults();

        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&qtTranslator);

        QTranslator appTranslator;
        appTranslator.load(QCoreApplication::applicationDirPath() + "/" + Globals::ApplicationName + "_" + QLocale::system().name());
        a.installTranslator(&appTranslator);

        PortEnumerator portEnumerator;
        ConsoleTabFactory::setPortEnumerator(&portEnumerator);

        qRegisterMetaTypeStreamOperators<Highlighting>("Highlighting");

        // create main window on heap!
        MainWindow* w = new MainWindow;
        w->addExistingTabsFromFile();
        w->show();

        QObject::connect(&a, &QApplication::aboutToQuit, w, &MainWindow::deleteLater);
        QObject::connect(&a, &SingleApplication::showUp, w, &MainWindow::onSecondaryInstanceLaunched);

        UpdateChecker updateChecker;
        updateChecker.checkForUpdate();

        QObject::connect(&updateChecker, &UpdateChecker::updateAvailable, w, &MainWindow::showUpdateInfo);

        return a.exec();
    }
}

// EOF <stefan@scheler.com>
