#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T18:58:27
#
#-------------------------------------------------

QT += core gui declarative network

TARGET = CitySync
TEMPLATE = app

INCLUDEPATH += Sc4SaveGame

INCLUDEPATH += C:\Boost\include\boost-1_53

SOURCES += main.cpp\
    logindialog.cpp \
    region.cpp \
    city.cpp \
    cityview.cpp \
    regionview.cpp \
    Sc4SaveGame/sc4savegame.cpp \
    Sc4SaveGame/regionviewfile.cpp \
    Sc4SaveGame/qfs.cpp \
    serverapi.cpp \
    server.cpp \
    regionselectdialog.cpp \
    jsoncpp.cpp

HEADERS  += regionview.h \
    logindialog.h \
    region.h \
    city.h \
    cityview.h \
    Sc4SaveGame/sc4savegame.h \
    Sc4SaveGame/regionviewfile.h \
    Sc4SaveGame/binaryreader.hpp \
    serverapi.h \
    server.h \
    regionselectdialog.h \
    json/json-forwards.h \
    json/json.h

FORMS    += regionview.ui \
    logindialog.ui \
    regionselectdialog.ui


LIBS += C:/Boost/lib/libboost_iostreams-mgw44-mt-1_53.a

OTHER_FILES += \
    tile.qml \
    resources/thumbnail.png \
    resources/tile.qml

RESOURCES += \
    res.qrc
