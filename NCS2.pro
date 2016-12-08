#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T20:25:07
#
#-------------------------------------------------

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NCS2
TEMPLATE = app
DEFINES += DEBUG

SOURCES += main.cpp\
        mainwindow.cpp \
        trioactivex.cpp \
    thread_ccd.cpp \
    thread_trio.cpp \
    common.cpp \
    thread_assist.cpp

HEADERS  += mainwindow.h \
            trioactivex.h \
    thread_ccd.h \
    thread_trio.h \
    common.h \
    thread_assist.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
