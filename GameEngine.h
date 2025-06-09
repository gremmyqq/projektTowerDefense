#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "EnemyRegular.h"
#include "Field.h"
#include "Hero.h"
#include "Castle.h"
#include "LevelLoader.h"

class GameEngine {
public:
    explicit GameEngine(sf::RenderWindow& window);
    void run();

private:
    sf::RenderWindow& window;

    // Komponenty gry
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Field> fields;
    std::unique_ptr<Hero> hero;
    Castle castle;

    //System ładowania poziomów
    LevelLoader levelLoader;

    std::vector<std::vector<sf::Vector2f>> allPaths;
    std::map<int, std::vector<EnemySpawn>> waveMap;

    int currentWave = 0;
    float waveTimer = 0.f;
    float spawnClock = 0.f; // zamiast waveTimer

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    size_t nextSpawnIndex = 0;

    // Ścieżka dla wrogów
    std::vector<sf::Vector2f> path;

    // Tekstury
    sf::Texture heroTexture;

    // Timery
    float spawnTimer;
    float spawnInterval;

    // Metody pomocnicze
    void handleEvents();
    void update(float deltaTime);
    void render();
    //void spawnEnemy();

};

#endif // GAMEENGINE_H
