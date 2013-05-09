#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T10:34:04
#
#-------------------------------------------------

QT       -= gui

TARGET = Sc4SaveGame
TEMPLATE = lib
CONFIG += staticlib

win32 {
    INCLUDEPATH += C:\development\boost_1_53_0
}

SOURCES += sc4savegame.cpp \
    regionviewfile.cpp \
    qfs.cpp

HEADERS += sc4savegame.h \
    regionviewfile.h \
    binaryreader.hpp
