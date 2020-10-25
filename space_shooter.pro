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
        shootableObject.cpp \
        stars.cpp \
        utilities.cpp

HEADERS += \
    alien.h \
    bullet.h \
    maintainers.h \
    ship.h \
    shootableObject.h \
    stars.h \
    utilities.h

DISTFILES += \
    README.md \
    explode1.flac \
    explode2.flac \
    shot.flac \
    spritesheet.png \
    ssconfig.ini

LIBS += -I/usr/include/x86_64-linux-gnu \
        -lallegro_image -lallegro_primitives \
        -lallegro_acodec -lallegro_audio \
        -lallegro_font \
        -lallegro

