TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        alien.cpp \
        bullet.cpp \
        main.cpp \
        maintainers.cpp \
        ship.cpp \
        utilities.cpp

HEADERS += \
    alien.h \
    bullet.h \
    maintainers.h \
    ship.h \
    utilities.h

DISTFILES += \
    explode1.flac \
    explode2.flac \
    shot.flac \
    spritesheet.png

LIBS += -I/usr/include/x86_64-linux-gnu \
        -lallegro_image -lallegro_primitives \
        -lallegro_acodec -lallegro_audio \
        -lallegro

