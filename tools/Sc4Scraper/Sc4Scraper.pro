TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp \
    Sc4CityFile.cpp \
    qfs.cpp


unix|win32: LIBS += -lboost_iostreams

HEADERS += \
    RegionViewFile.h \
    Sc4CityFile.h

OTHER_FILES += \
    testdata/RegionView_SteamboatSprings
