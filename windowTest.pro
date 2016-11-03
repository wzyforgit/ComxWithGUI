#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T16:54:25
#
#-------------------------------------------------

QT       += core gui \
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = windowTest
TEMPLATE = app

CONFIG += x86

SOURCES += \
    mainWindow.cpp \
    main.cpp \
    common.cpp

HEADERS  += \
    mainWindow.h \
    common.h

DISTFILES += \
    s5.png
