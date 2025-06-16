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
        EnemyArcher.cpp \
        EnemyBoss.cpp \
        EnemyFast.cpp \
        EnemyTank.cpp \
        GameEngine.cpp \
        GeneratorField.cpp \
        HeroBase.cpp \
        Knight.cpp \
        LevelLoader.cpp \
        Shop.cpp \
        TowerArcher.cpp \
        TowerCatapult.cpp \
        TowerField.cpp \
        TowerWizard.cpp \
        arrow.cpp \
        mage.cpp \
        main.cpp \
        Enemy.cpp \
        EnemyRegular.cpp \
        samurai.cpp \
        spell.cpp

HEADERS += \
        Archer.h \
        BuildType.h \
        Castle.h \
        EmptyField.h \
        Enemy.h \
        EnemyArcher.h \
        EnemyBoss.h \
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
        TowerCatapult.h \
        TowerField.h \
        TowerWizard.h \
        arrow.h \
        mage.h \
        samurai.h \
        spell.h
