#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "EnemyRegular.h"
#include "Field.h"
#include "Hero.h"
#include "Castle.h"

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
    void spawnEnemy();
};

#endif // GAMEENGINE_H
