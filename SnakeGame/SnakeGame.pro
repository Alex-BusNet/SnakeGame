#-------------------------------------------------
#
# Project created by QtCreator 2017-02-23T14:28:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SnakeGame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamemanager.cpp \
    grid.cpp \
    tile.cpp

HEADERS  += mainwindow.h \
    gamemanager.h \
    grid.h \
    tile.h \
    food.h \
    datatypes.h

FORMS    += mainwindow.ui
