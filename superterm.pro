QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = superterm
TEMPLATE = app

DESTDIR = build/release
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
UI_DIR  = $$DESTDIR/.ui
RCC_DIR = $$DESTDIR/.rcc

SOURCES += src/main.cpp \
           src/ui/mainwindow.cpp \
           src/ui/consoleview.cpp \
    src/ui/newconnectiondialog.cpp \
    src/consolemgr.cpp


HEADERS += src/ui/mainwindow.h \
           src/ui/consoleview.h \
    src/ui/newconnectiondialog.h \
    src/consolemgr.h

FORMS   += src/ui/mainwindow.ui \
    src/ui/consoleview.ui \
    src/ui/newconnectiondialog.ui

RESOURCES += \
    superterm.qrc
