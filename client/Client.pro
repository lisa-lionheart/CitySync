#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T18:58:27
#
#-------------------------------------------------

QT += core gui declarative network webkit
DEFINES += QT_STATIC_BUILD

#DEPENDPATH += resources

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
    jsoncpp.cpp \
    gamesession.cpp \
    bspatch.c \
    bsdiff.c

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
    json/json.h \
    gamesession.h

FORMS    += regionview.ui \
    logindialog.ui \
    regionselectdialog.ui

LIBS += -static -static-libgcc

LIBS += C:/Boost/lib/libboost_iostreams-mgw44-mt-1_53.a

OTHER_FILES += \
    tile.qml \
    resources/thumbnail.png \
    resources/tile.qml \
    resources/overlay.qml \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml

RESOURCES += \
    res.qrc
