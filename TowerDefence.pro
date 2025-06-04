TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "C:\Users\CP\Documents\Qt Laby\SFML-2.5.1\SFML-2.5.1\include"

LIBS += -L"C:\Users\CP\Documents\Qt Laby\SFML-2.5.1\SFML-2.5.1\lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}
SOURCES += \
        main.cpp \
        Enemy.cpp \
        EnemyRegular.cpp

HEADERS += \
        Enemy.h \
        EnemyRegular.h



