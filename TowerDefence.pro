TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "C:\SFML\include"

LIBS += -LC:\SFML\lib
LIBS += -lsfml-graphics -lsfml-window -lsfml-system

assets.files = $$PWD/assets/*
assets.path = $$OUT_PWD/assets
INSTALLS += assets

SOURCES += \
        Archer.cpp \
        Castle.cpp \
        EnemyFast.cpp \
        EnemyTank.cpp \
        Field.cpp \
        GameEngine.cpp \
        HeroBase.cpp \
        Knight.cpp \
        LevelLoader.cpp \
        Shop.cpp \
        Tower.cpp \
        mage.cpp \
        main.cpp \
        Enemy.cpp \
        EnemyRegular.cpp

HEADERS += \
        Archer.h \
        Castle.h \
        Enemy.h \
        EnemyFast.h \
        EnemyRegular.h \
        EnemyTank.h \
        Field.h \
        GameEngine.h \
        HeroBase.h \
        Knight.h \
        LevelLoader.h \
        Shop.h \
        Tower.h \
        mage.h
