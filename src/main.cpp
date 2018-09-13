#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDebug>

#include "misc/globals.h"
#include "misc/defaultsettings.h"
#include "misc/usagetracker.h"
#include "ui/mainwindow.h"
#include "ui/consoletabfactory.h"
#include "ui/highlightsframe.h"
#include "serial/portapplication.h"
#include "serial/portenumerator.h"
#include "singleapplication/singleapplication.h"

#include "misc/softwareversion.h"

int main(int argc, char* argv[])
{
    SoftwareVersion v1("2018.8a");
    SoftwareVersion v2("2018.8b");

    qDebug() << (v2 > v1);


    if (argc == 7)
    {
        CPortApplication a(argc, argv);
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

        CPortEnumerator portEnumerator;
        CConsoleTabFactory::setPortEnumerator(&portEnumerator);

        qRegisterMetaTypeStreamOperators<CHighlightsFrame::Highlighting>("CHighlightsFrame::Highlighting");

        // create main window on heap!
        CMainWindow* w = new CMainWindow;
        w->addExistingTabsFromFile();
        w->show();

        QObject::connect(&a, &QApplication::aboutToQuit, w, &CMainWindow::deleteLater);
        QObject::connect(&a, &SingleApplication::showUp, w, &CMainWindow::onSecondaryInstanceLaunched);

        UsageTracker usageTracker;
        usageTracker.trackUsage();

        return a.exec();
    }
}

// EOF <stefan@scheler.com>
