cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/SFML) # jeśli używasz submodułu

add_executable(TowerDefense main.cpp src/Enemy.cpp src/BasicEnemy.cpp)

target_include_directories(TowerDefense PRIVATE include)

target_link_libraries(TowerDefense
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)
