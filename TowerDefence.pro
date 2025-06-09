TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "C:\SFML\include"

LIBS += -LC:\SFML\lib
LIBS += -lsfml-graphics -lsfml-window -lsfml-system


SOURCES += \
        Castle.cpp \
        Field.cpp \
        GameEngine.cpp \
        Hero.cpp \
        LevelLoader.cpp \
        Shop.cpp \
        Tower.cpp \
        main.cpp \
        Enemy.cpp \
        EnemyRegular.cpp

HEADERS += \
        Castle.h \
        Enemy.h \
        EnemyRegular.h \
        Field.h \
        GameEngine.h \
        Hero.h \
        LevelLoader.h \
        Shop.h \
        Tower.h



