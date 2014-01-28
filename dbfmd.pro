#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T20:21:19
#
#-------------------------------------------------

QT       += core network multimedia

QT       -= gui

macx {
    # QMediaPlayer not work with QCoreApplication on Mac OS X
    QT += gui
}

TARGET = dbfmd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/login.cpp \
    src/client.cpp \
    src/operationprocessor.cpp \
    src/control.cpp \
    src/player.cpp \
    src/remotecontrol.cpp

HEADERS += \
    src/login.h \
    src/client.h \
    src/operationprocessor.h \
    src/control.h \
    src/player.h \
    src/remotecontrol.h \
    include/nzmqt/nzmqt.hpp

INCLUDEPATH += include

# please create a symbol link of libzmq.a
LIBS += $$system(PWD)/libzmq.a
