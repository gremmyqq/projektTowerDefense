#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "EnemyRegular.h"
#include "Tower.h"

class GameEngine {
public:
    GameEngine(sf::RenderWindow& window);
    void run();

private:
    sf::RenderWindow& window;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Tower> towers;
    std::vector<sf::Vector2f> path;

    float spawnTimer;
    float spawnInterval;

    void handleEvents();
    void update(float deltaTime);
    void render();

    void spawnEnemy();
};

#endif // GAMEENGINE_H
