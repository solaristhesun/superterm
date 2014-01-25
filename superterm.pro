QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = superterm
TEMPLATE = app

DESTDIR = build/release
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
UI_DIR  = $$DESTDIR/.ui
RCC_DIR = $$DESTDIR/.rcc

SOURCES += \
    src/main.cpp \
    src/ui/mainwindow.cpp \
    src/consolemgr.cpp \
    src/ui/consoletabwidget.cpp \
    src/ui/consoleview.cpp \
    src/ui/consoletab.cpp


HEADERS += \
    src/ui/mainwindow.h \
    src/consolemgr.h \
    src/ui/consoletabwidget.h \
    src/ui/consoleview.h \
    src/ui/consoletab.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/consoleview.ui \
    src/ui/consoletab.ui

RESOURCES += \
    superterm.qrc

win32:INCLUDEPATH += C:\DUMP\projects\superterm\
