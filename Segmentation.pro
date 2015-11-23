#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T17:34:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Segmentation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mywidget.cpp

HEADERS  += mainwindow.h \
    mywidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/Eigen
