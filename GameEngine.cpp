#include "GameEngine.h"

GameEngine::GameEngine(sf::RenderWindow& window)
    : window(window), spawnTimer(0.f), spawnInterval(3.f) {
    path = {{100, 100}, {700, 100}, {700, 500}, {100, 500}, {100, 300}};
    towers.emplace_back(sf::Vector2f(400.f, 300.f));
}

void GameEngine::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        handleEvents();
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);
        render();
    }
}

void GameEngine::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void GameEngine::update(float deltaTime) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnEnemy();
        spawnTimer = 0.f;
    }

    for (auto& enemy : enemies)
        enemy.update(deltaTime);

    for (auto& tower : towers)
        tower.update(deltaTime, enemies);

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const std::unique_ptr<Enemy>& e) {
                                     return e->isDead() || e->reachedEnd();
                                 }), enemies.end());
}

void GameEngine::render() {
    window.clear();

    for (auto& tower : towers)
        tower.draw(window);

    for (auto& enemy : enemies)
        enemy->draw(window);

    window.display();
}

void GameEngine::spawnEnemy() {
    enemies.emplace_back(std::make_unique<EnemyRegular>(path));
}
