#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T18:58:27
#
#-------------------------------------------------

QT += core gui

TARGET = CitySync
TEMPLATE = app

INCLUDEPATH += ..\Sc4SaveGame

INCLUDEPATH += C:\Boost\include\boost-1_53

SOURCES += main.cpp\
    logindialog.cpp \
    region.cpp \
    city.cpp \
    cityview.cpp \
    regionview.cpp \
    ../Sc4SaveGame/sc4savegame.cpp \
    ../Sc4SaveGame/regionviewfile.cpp \
    ../Sc4SaveGame/qfs.cpp \
    serverapi.cpp

HEADERS  += regionview.h \
    logindialog.h \
    region.h \
    city.h \
    cityview.h \
    ../Sc4SaveGame/sc4savegame.h \
    ../Sc4SaveGame/regionviewfile.h \
    ../Sc4SaveGame/binaryreader.hpp \
    serverapi.h

FORMS    += regionview.ui \
    logindialog.ui


LIBS += C:/Boost/lib/libboost_iostreams-mgw44-mt-1_53.a
