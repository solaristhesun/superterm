QT       += core gui serialport network widgets

CONFIG += c++11

TARGET = supertermtests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    googletest\include googletest \
    googlemock\include googlemock \
    ..\src

SOURCES += \
    googletest\src\gtest-all.cc \
    googlemock\src\gmock-all.cc \
    sessiontests.cpp \
    main.cpp \
    statusbarframetests.cpp

SOURCES += \
    ../src/session/session.cpp \
    ../src/session/serializableobject.cpp \
    ../src/ui/statusbarframe.cpp \
    ../src/ui/waitingspinnerwidget.cpp

HEADERS += \
    ../src/session/session.h \
    ../src/session/serializableobject.h \
    ../src/ui/statusbarframe.h \
    ../src/ui/waitingspinnerwidget.h

FORMS += \
    ../src/ui/statusbarframe.ui
