QT       += core gui serialport network

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
    src/ui/consoletabwidget.cpp \
    src/ui/consoletab.cpp \
    src/ui/consoletabbar.cpp \
    src/ui/statusbarframe.cpp \
    src/ui/portscombobox.cpp \
    src/ui/highlightingsframe.cpp \
    src/ui/consoletabfactory.cpp \
    src/ui/logfilepanel.cpp \
    src/misc/globals.cpp \
    src/serial/portenumerator.cpp \
    src/serial/serialportinfo.cpp \
    src/serial/portendpoint.cpp \
    src/serial/portapplication.cpp \
    src/serial/mockportapp.cpp \
    src/session/session.cpp \
    src/session/serializableobject.cpp \
    src/ipc/message.cpp \
    src/ipc/messagecodec.cpp \
    src/serial/portobserver.cpp \
    src/ui/connectionbar.cpp \
    src/misc/defaultsettings.cpp \
    src/ui/renametabframe.cpp \
    src/misc/uniqueidentifier.cpp \
    src/ui/portitemdelegate.cpp \
    src/ui/portitemview.cpp \
    src/misc/softwareversion.cpp \
    src/misc/updatechecker.cpp \
    src/ui/notificationbar.cpp \
    src/ui/widgets/clickablelabel.cpp \
    src/models/consolelinebuffer.cpp \
    src/ui/consoleview/consoleview.cpp \
    src/models/consoleline.cpp \
    src/ui/consoleview/consolelineitemdelegate.cpp \
    src/models/highlightingsmodel.cpp

HEADERS += \
    src/ui/mainwindow.h \
    src/ui/consoletabwidget.h \
    src/ui/consoletab.h \
    src/ui/consoletabbar.h \
    src/ui/statusbarframe.h \
    src/ui/portscombobox.h \
    src/ui/highlightingsframe.h \
    src/ui/consoletabfactory.h \
    src/ui/logfilepanel.h \
    src/misc/globals.h \
    src/serial/portenumerator.h \
    src/serial/serialportinfo.h \
    src/serial/portendpoint.h \
    src/serial/portapplication.h \
    src/serial/mockportapp.h \
    src/session/session.h \
    src/session/serializableobject.h \
    src/ipc/message.h \
    src/ipc/messagecodec.h \
    src/serial/portobserver.h \
    src/ui/connectionbar.h \
    src/misc/defaultsettings.h \
    src/ui/renametabframe.h \
    src/misc/uniqueidentifier.h \
    src/ui/portitemdelegate.h \
    src/ui/portitemview.h \
    src/misc/softwareversion.h \
    src/misc/updatechecker.h \
    src/ui/notificationbar.h \
    src/ui/widgets/clickablelabel.h \
    src/models/consolelinebuffer.h \
    src/ui/consoleview/consoleview.h \
    src/models/consoleline.h \
    src/ui/consoleview/consolelineitemdelegate.h \
    src/models/highlighting.h \
    src/models/highlightingsmodel.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/statusbarframe.ui \
    src/ui/highlightingsframe.ui \
    src/ui/logfilepanel.ui \
    src/ui/connectionbar.ui \
    src/ui/consoletab.ui \
    src/ui/renametabframe.ui \
    src/ui/portitemview.ui \
    src/ui/notificationbar.ui \
    src/ui/consoleview/consoleview.ui

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

include(contrib/singleapplication/singleapplication.pri)
include(contrib/waitingspinnerwidget/waitingspinnerwidget.pri)

win32:INCLUDEPATH += C:\\DUMP\\projects\\superterm
win32:RC_FILE += superterm.rc
win32:CONFIG += embed_manifest_exe

CONFIG += c++11

INCLUDEPATH += \
    src \
    contrib

REVISION = $$system(git rev-list --count HEAD)

isEmpty(REVISION){
    REVISION = 0
}

DEFINES += REVISION=$${REVISION} 

# EOF <stefan@scheler.com>

STATECHARTS +=

