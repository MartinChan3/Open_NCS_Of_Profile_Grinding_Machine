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

win32: LIBS += -L$$PWD/'../../../../../Program Files (x86)/Mvision/Mvision SDK/Mvision SDK/lib/' -lMvCameraControl

INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/Mvision/Mvision SDK/Mvision SDK/inc'
DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/Mvision/Mvision SDK/Mvision SDK/inc'

win32: LIBS += -L$$PWD/'../../../../../Program Files/GenICam_v2_4/library/CPP/lib/Win32_i86/' -lGenApi_MD_VC80_v2_4

INCLUDEPATH += $$PWD/'../../../../../Program Files/GenICam_v2_4/library/CPP/include'
DEPENDPATH += $$PWD/'../../../../../Program Files/GenICam_v2_4/library/CPP/include'
