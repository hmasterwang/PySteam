TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
TARGET = pysteam

SOURCES += \
    steamwrapper.cpp \
    pysteam.cpp \
    achievements.cpp \
    achievements_defs.cpp

INCLUDEPATH += C:\Python27\include \
                D:\SDK\steamworks_sdk_132\sdk\public

LIBS += -LD:\SDK\renpy-6.18.3-sdk\lib\windows-i686 \
        -lpython27 \
        -lsteam_api

HEADERS += \
    achievements.h \
    global.h
