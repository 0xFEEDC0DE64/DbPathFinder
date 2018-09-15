QT += core gui widgets

DBLIBS +=

TARGET = pathfinder

PROJECT_ROOT = ..

SOURCES += main.cpp \
    mywidget.cpp \
    mainwindow.cpp

HEADERS += \
    mywidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES +=

TRANSLATIONS +=

include($${PROJECT_ROOT}/app.pri)
