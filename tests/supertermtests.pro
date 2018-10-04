QT       += core gui serialport network widgets testlib

CONFIG += c++11

TARGET = supertermtests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    googletest \
    googletest/include \
    googlemock \
    googlemock/include \
    ../src \
    ../contrib

SOURCES += \
    googletest/src/gtest-all.cc \
    googlemock/src/gmock-all.cc \
    sessiontests.cpp \
    main.cpp \
    statusbarframetests.cpp \
    highlightingsmodeltests.cpp \
    consolelinebuffertests.cpp

SOURCES += \
    ../src/session/session.cpp \
    ../src/session/serializableobject.cpp \
    ../src/ui/statusbarframe.cpp \
    ../src/models/highlightingsmodel.cpp \
    ../src/models/consolelinebuffer.cpp \
    ../src/models/consoleline.cpp \
    ../src/ui/consoleview/consoleview.cpp \
    ../src/ui/consoleview/consolelineitemdelegate.cpp \
    ../contrib/waitingspinnerwidget/waitingspinnerwidget.cpp

HEADERS += \
    ../src/session/session.h \
    ../src/session/serializableobject.h \
    ../src/ui/statusbarframe.h \
    ../src/models/highlightingsmodel.h \
    ../src/models/consolelinebuffer.h \
    ../src/models/consoleline.h \
    ../src/ui/consoleview/consoleview.h \
    ../src/ui/consoleview/consolelineitemdelegate.h \
    ../contrib/waitingspinnerwidget/waitingspinnerwidget.h

FORMS += \
    ../src/ui/statusbarframe.ui \
    ../src/ui/consoleview/consoleview.ui \
