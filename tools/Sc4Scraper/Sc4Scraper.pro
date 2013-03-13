TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp

QMAKE_LIBDIR += ../Sc4SaveGame-build-Desktop-Debug
INCLUDEPATH += ../Sc4SaveGame

unix|win32: LIBS += -lboost_iostreams -lSc4Savegame

HEADERS +=

OTHER_FILES += \
    testdata/RegionView_SteamboatSprings
