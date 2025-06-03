cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)

# Wskaż SFML
set(SFML_DIR "C:/Libraries/SFML/lib/cmake/SFML")

# Znajdź SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Twój program
add_executable(TowerDefense
        main.cpp
        Enemy.cpp
        EnemyRegular.cpp
)

# Nagłówki użytkownika
target_include_directories(TowerDefense PRIVATE .)

# Linkuj biblioteki SFML
target_link_libraries(TowerDefense PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

# Skopiuj DLL-e po kompilacji
add_custom_command(TARGET TowerDefense POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Libraries/SFML/bin/sfml-graphics-2.dll"
        "C:/Libraries/SFML/bin/sfml-window-2.dll"
        "C:/Libraries/SFML/bin/sfml-system-2.dll"
        $<TARGET_FILE_DIR:TowerDefense>
)
