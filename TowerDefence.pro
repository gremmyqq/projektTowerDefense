TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "C:\SFML\include"

LIBS += -L"C:\SFML\lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}
SOURCES += \
        GameEngine.cpp \
        Hero.cpp \
        main.cpp \
        Enemy.cpp \
        EnemyRegular.cpp

HEADERS += \
        Enemy.h \
        EnemyRegular.h \
        GameEngine.h \
        Hero.h



