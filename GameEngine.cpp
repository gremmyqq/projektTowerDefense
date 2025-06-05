#include "GameEngine.h"
#include <stdexcept>

GameEngine::GameEngine(sf::RenderWindow& window)
    : window(window),
    spawnTimer(0.f),
    spawnInterval(3.f),
    castle(sf::Vector2f(400.f, 500.f)) {

    // Ścieżka dla wrogów (prosta trasa)
    path = {
        {100, 100}, {700, 100}, {700, 500}, {100, 500}, {100, 300}
    };

    // Inicjalizacja wież
    towers.emplace_back(sf::Vector2f(300.f, 300.f));
    towers.emplace_back(sf::Vector2f(500.f, 300.f));

    // Wczytanie tekstury bohatera
    if (!heroTexture.loadFromFile("assets/hero.png")) {
        throw std::runtime_error("Nie można załadować assets/hero.png");
    }

    // Tworzenie bohatera
    hero = std::make_unique<Hero>(sf::Vector2f(400.f, 500.f), heroTexture);
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

    // Aktualizacja przeciwników
    for (auto& enemy : enemies)
        enemy->update(deltaTime);

    // Bohater
    if (hero)
        hero->update(deltaTime, window, enemies);

    // Wieże
    for (auto& tower : towers) {
        tower.update(deltaTime);
        for (auto& enemy : enemies) {
            tower.attack(enemy.get()); // ✅ teraz 'enemy' istnieje
        }
    }

    // Zamek
    castle.update();

    // Uszkodzenie zamku przez przeciwników
    for (const auto& enemy : enemies) {
        if (enemy->reachedEnd()) {
            castle.takeDamage(50); // przyklad
        }
    }

    // Usunięcie martwych wrogów
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const std::unique_ptr<Enemy>& e) {
                                     return e->isDead() || e->reachedEnd();
                                 }), enemies.end());
}

void GameEngine::render() {
    window.clear();

    // Zamek
    castle.draw(window);

    // Wieże
    for (auto& tower : towers)
        tower.draw(window);

    // Bohater
    if (hero)
        hero->draw(window);

    // Przeciwnicy
    for (auto& enemy : enemies)
        enemy->draw(window);

    window.display();
}

void GameEngine::spawnEnemy() {
    enemies.emplace_back(std::make_unique<EnemyRegular>(path));
}
