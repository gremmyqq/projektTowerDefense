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
        EmptyField.cpp \
        EnemyFast.cpp \
        EnemyTank.cpp \
        GameEngine.cpp \
        GeneratorField.cpp \
        HeroBase.cpp \
        Knight.cpp \
        LevelLoader.cpp \
        Shop.cpp \
        TowerArcher.cpp \
        TowerField.cpp \
        mage.cpp \
        main.cpp \
        Enemy.cpp \
        EnemyRegular.cpp

HEADERS += \
        Archer.h \
        BuildType.h \
        Castle.h \
        EmptyField.h \
        Enemy.h \
        EnemyFast.h \
        EnemyRegular.h \
        EnemyTank.h \
        Field.h \
        GameEngine.h \
        GeneratorField.h \
        HeroBase.h \
        Knight.h \
        LevelLoader.h \
        Shop.h \
        TowerArcher.h \
        TowerField.h \
        mage.h
