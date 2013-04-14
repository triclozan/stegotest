#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T18:13:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stegotest
TEMPLATE = app

INCLUDEPATH += $$PWD/../target
INCLUDEPATH += $$PWD/src/h

SOURCES += src/cpp/mainwindow.cpp \
    src/cpp/main.cpp

HEADERS  += src/h/mainwindow.h

FORMS    += src/ui/mainwindow.ui
