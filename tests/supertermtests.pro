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
    highlightingsmodeltests.cpp

SOURCES += \
    ../src/session/session.cpp \
    ../src/session/serializableobject.cpp \
    ../src/ui/statusbarframe.cpp \
    ../src/models/highlightingsmodel.cpp \
    ../contrib/waitingspinnerwidget/waitingspinnerwidget.cpp

HEADERS += \
    ../src/session/session.h \
    ../src/session/serializableobject.h \
    ../src/ui/statusbarframe.h \
    ../src/models/highlightingsmodel.h \
    ../contrib/waitingspinnerwidget/waitingspinnerwidget.h

FORMS += \
    ../src/ui/statusbarframe.ui
