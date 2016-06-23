QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = superterm
TEMPLATE = app

DESTDIR = release
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
UI_DIR  = $$DESTDIR/.ui
RCC_DIR = $$DESTDIR/.rcc

SOURCES += \
    src/main.cpp \
    src/ui/mainwindow.cpp \
    src/ui/consoletabwidget.cpp \
    src/ui/consoleview.cpp \
    src/ui/consoletab.cpp \
    src/ui/consoletabbar.cpp \
    src/ui/statusbarframe.cpp \
    src/ui/portscombobox.cpp \
    src/ui/highlightsframe.cpp \
    src/globals.cpp \
    src/ui/logfilepanel.cpp \
    src/enumerator/portenumerator.cpp \
    src/enumerator/serialportinfo.cpp \
    src/obj/seriazableobject.cpp \
    src/port/portendpoint.cpp \
    src/port/portapplication.cpp \
    src/obj/session.cpp \
    src/ui/consoleviewex.cpp

HEADERS += \
    src/ui/mainwindow.h \
    src/ui/consoletabwidget.h \
    src/ui/consoleview.h \
    src/ui/consoletab.h \
    src/ui/consoletabbar.h \
    src/ui/statusbarframe.h \
    src/ui/portscombobox.h \
    src/ui/highlightsframe.h \
    src/globals.h \
    src/ui/logfilepanel.h \
    src/enumerator/portenumerator.h \
    src/enumerator/serialportinfo.h \
    src/obj/seriazableobject.h \
    src/port/portendpoint.h \
    src/port/portapplication.h \
    src/obj/session.h \
    src/ui/consoleviewex.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/consoleview.ui \
    src/ui/consoletab.ui \
    src/ui/statusbarframe.ui \
    src/ui/highlightsframe.ui \
    src/ui/logfilepanel.ui

RESOURCES += \
    superterm.qrc

OTHER_FILES += \
    superterm.qss

TRANSLATIONS += \
    languages/superterm_de.ts

updateqm.input = TRANSLATIONS
updateqm.output = $$DESTDIR/${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT} 
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

win32:INCLUDEPATH += C:\\DUMP\\projects\\superterm
win32:RC_FILE += superterm.rc
win32:CONFIG += embed_manifest_exe

CONFIG += c++11

INCLUDEPATH += \
    src

REVISION = $$system(git rev-list --count HEAD)

isEmpty(REVISION){
    REVISION = 0
}

DEFINES += REVISION=$${REVISION} 

# EOF <stefan@scheler.com>

