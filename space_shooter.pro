TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    alien.h \
    bullet.h \
    ship.h \
    utilities.h

DISTFILES += \
    explode1.flac \
    explode2.flac \
    shot.flac \
    spritesheet.png

LIBS += -I/usr/include/x86_64-linux-gnu -lallegro_image -lallegro

