#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T20:21:19
#
#-------------------------------------------------

QT       += core network multimedia

QT       -= gui

TARGET = dbfmd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += src/main.cpp \
    src/login.cpp \
    src/client.cpp \
    src/operationprocessor.cpp \
    src/control.cpp \
    src/player.cpp

HEADERS += \
    src/login.h \
    src/client.h \
    src/operationprocessor.h \
    src/control.h \
    src/player.h
