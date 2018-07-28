TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
#    ../dwmstatus/profil-dwmstatus-1.0.c \
#    ../dwmstatus/dwmstatus.c
    Display.cpp

LIBS += \
        -lX11

HEADERS += \
    Display.h
