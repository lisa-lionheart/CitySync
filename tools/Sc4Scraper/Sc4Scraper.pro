TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static

INCLUDEPATH += ../../Sc4SaveGame/

SOURCES += main.cpp \
    ../../Sc4SaveGame/sc4savegame.cpp \
    ../../Sc4SaveGame/regionviewfile.cpp \
    ../../Sc4SaveGame/qfs.cpp


win32:INCLUDEPATH += C:\Boost\include\boost-1_53
win32: LIBS += C:\Boost\lib\libboost_iostreams-mgw44-mt-1_53.a C:\Boost\lib\libboost_program_options-mgw44-mt-1_53.a


HEADERS += \
    ../../Sc4SaveGame/sc4savegame.h \
    ../../Sc4SaveGame/regionviewfile.h \
    ../../Sc4SaveGame/binaryreader.hpp

OTHER_FILES += \
    testdata/RegionView_SteamboatSprings
