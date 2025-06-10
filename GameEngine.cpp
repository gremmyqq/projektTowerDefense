#include "GameEngine.h"
#include <stdexcept>

GameEngine::GameEngine(sf::RenderWindow& window)
    : window(window),
    spawnTimer(0.f),
    spawnInterval(3.f),
    castle(sf::Vector2f(600.f, 600.f)) {

    /*
    path = {
        {window.getSize().x * 0.1f, window.getSize().y * 0.1f},
        {window.getSize().x * 0.7f, window.getSize().y * 0.1f},
        {window.getSize().x * 0.7f, window.getSize().y * 0.5f},
        {window.getSize().x * 0.1f, window.getSize().y * 0.5f},
        {window.getSize().x * 0.1f, window.getSize().y * 0.3f}
    };
    */
    fields.emplace_back(sf::Vector2f(window.getSize().x * 0.3f, window.getSize().y * 0.3f));
    fields.emplace_back(sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y * 0.3f));


    // Wczytanie tekstury bohatera
    if (!heroTexture.loadFromFile("assets/hero1.png")) {
        throw std::runtime_error("Nie można załadować assets/hero.png");
    }

    // Tworzenie bohatera
    hero = std::make_unique<Hero>(sf::Vector2f(400.f, 500.f), heroTexture);

    // Ładowanie rund
    if (!levelLoader.loadFromFile("assets/levels.txt")) {
        throw std::runtime_error("Nie można załadować levels.txt");
    }

    //Ładowanie tła
    if (!backgroundTexture.loadFromFile("assets/tlomapa.bmp")) {
        throw std::runtime_error("Nie można załadować tła: assets/tlomapa.bmp");
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );


    allPaths = levelLoader.getPaths();
    waveMap = levelLoader.getWaveMap();

    uiFont.loadFromFile("assets/arial.ttf"); // lub inny font
    shop.setFont(uiFont);
    shop.setPosition({20.f, 500.f});
    shop.setGoldPointer(&playerResources);

    shop.addItem("Wieża", 50, [this]() {
        if (selectedField) selectedField->buildTower();
        selectedField = nullptr;
    });

    shop.addItem("Generator", 30, [this]() {
        if (selectedField) selectedField->buildGenerator();
        selectedField = nullptr;
    });

    shop.addItem("Ulepsz zamek", 100, [this]() {
        if (castle.getLevel() < castle.getMaxLevel()) {
            castle.upgrade();
            std::cout << "Zamek ulepszony do poziomu " << castle.getLevel() << std::endl;
        } else {
            std::cout << "Zamek ma już maksymalny poziom." << std::endl;
        }
    });


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
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {

            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            selectedField = nullptr;
            for (auto& field : fields) {
                if (field.contains(mousePos)) {
                    if (field.getType() == FieldType::Empty) {
                        selectedField = &field;
                    }
                    field.handleClick(mousePos);
                    break;
                }
            }

            shop.handleClick(mousePos);
        }


    }
}


void GameEngine::update(float deltaTime) {
    spawnClock += deltaTime;

    while (waveMap.find(currentWave) != waveMap.end()) {
        const auto& wave = waveMap[currentWave];

        if (nextSpawnIndex < wave.size() &&
            spawnClock >= wave[nextSpawnIndex].delay) {
            std::cout <<"test1"<<std::endl;

            const auto& spawn = wave[nextSpawnIndex];

            if (spawn.pathIndex >= 0 && spawn.pathIndex < static_cast<int>(allPaths.size())) {

                std::cout << "spawn.type = '" << spawn.type << "'" << std::endl;
                if (spawn.type == " Regular") {
                    enemies.emplace_back(std::make_unique<EnemyRegular>(allPaths[spawn.pathIndex]));
                    std::cout << "SPAWN " << nextSpawnIndex << " at path " << spawn.pathIndex << std::endl;
                    std::cout << "[ENEMY SPAWNED] at (" << enemies[nextSpawnIndex]->getPosition().x << "," << enemies[nextSpawnIndex]->getPosition().y << ")" << std::endl;

                }
            }

            ++nextSpawnIndex;
        } else {
            break;
        }

        if (nextSpawnIndex >= wave.size() && enemies.empty()) {
            ++currentWave;
            nextSpawnIndex = 0;
            spawnClock = 0.f;
            std::cout << "Next wave: " << currentWave << std::endl;
        }
    }





    // Aktualizacja przeciwników
    for (auto& enemy : enemies)
        enemy->update(deltaTime);

    // Bohater
    if (hero)
        hero->update(deltaTime, window, enemies);

    // Wieże
    for (auto& field : fields) {
        field.update(deltaTime, enemies); // ✔️ przekazujemy enemy tylko raz
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
    window.draw(backgroundSprite);

    // Zamek
    castle.draw(window);

    // Wieże
    for (auto& field : fields)
        field.draw(window);

    // Bohater
    if (hero)
        hero->draw(window);

    // Przeciwnicy
    for (auto& enemy : enemies)
        enemy->draw(window);

    if (selectedField)
        shop.draw(window);


    window.display();
}

//void GameEngine::spawnEnemy() {
//    enemies.emplace_back(std::make_unique<EnemyRegular>(path));
//}
