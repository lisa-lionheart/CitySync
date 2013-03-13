#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T10:34:04
#
#-------------------------------------------------

QT       -= gui

TARGET = Sc4SaveGame
TEMPLATE = lib
CONFIG += staticlib

SOURCES += sc4savegame.cpp \
    regionviewfile.cpp \
    qfs.cpp

HEADERS += sc4savegame.h \
    regionviewfile.h \
    binaryreader.hpp
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
