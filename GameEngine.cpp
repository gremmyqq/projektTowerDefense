#include "GameEngine.h"
#include <stdexcept>
#include <iostream>
#include "Archer.h"
#include "Knight.h"
#include "Mage.h"

GameEngine::GameEngine(sf::RenderWindow& window)
    : window(window),
    spawnTimer(0.f),
    spawnInterval(3.f),
    castle(sf::Vector2f(280.f, 1080.f)) {

    if (!heroTexture.loadFromFile("assets/hero1.png")) {
        throw std::runtime_error("Nie można załadować assets/hero1.png");
    }

    if (!backgroundTexture.loadFromFile("assets/tlomapa.bmp")) {
        throw std::runtime_error("Nie można załadować tła: assets/tlomapa.bmp");
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );

    if (!startButtonTexture.loadFromFile("assets/start_button.bmp")) {
        throw std::runtime_error("Nie można załadować start_button.bmp");
    }
    startButtonSprite.setTexture(startButtonTexture);
    startButtonSprite.setOrigin(
        startButtonTexture.getSize().x / 2.f,
        startButtonTexture.getSize().y / 2.f
        );
    startButtonSprite.setPosition(window.getSize().x - 100.f, window.getSize().y - 100.f);

    if (!levelLoader.loadFromFile("assets/levels.txt")) {
        throw std::runtime_error("Nie można załadować levels.txt");
    }
    allPaths = levelLoader.getPaths();
    waveMap = levelLoader.getWaveMap();

    for (auto& path : allPaths) {
        for (auto& point : path) {
            point.x = (point.x / 1280.f) * window.getSize().x;
            point.y = (point.y / 655.f) * window.getSize().y;
        }
    }

    uiFont.loadFromFile("assets/arial.ttf");

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
        }
    });

    fields.emplace_back(sf::Vector2f(window.getSize().x * 0.3f, window.getSize().y * 0.3f));
    fields.emplace_back(sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y * 0.3f));

    initHeroSelectionUI();
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
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            if (startButtonSprite.getGlobalBounds().contains(mousePos) && !roundActive) {
                roundActive = true;
                spawnClock = 0;
            }

            handleHeroSelectionClick(mousePos);
            if (hero) {
                hero->queueAttack();
            }

            if (!hero) return;

            selectedField = nullptr;
            for (auto& field : fields) {
                if (field.contains(mousePos)) {
                    if (field.getType() == FieldType::Empty)
                        selectedField = &field;
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

    if (roundActive && waveMap.find(currentWave) != waveMap.end()) {
        const auto& wave = waveMap[currentWave];

        if (nextSpawnIndex < wave.size() &&
            spawnClock >= wave[nextSpawnIndex].delay) {

            const auto& spawn = wave[nextSpawnIndex];
            if (spawn.pathIndex >= 0 && spawn.pathIndex < static_cast<int>(allPaths.size())) {
                if (spawn.type == " Regular") {
                    enemies.emplace_back(std::make_unique<EnemyRegular>(allPaths[spawn.pathIndex]));
                }
            }

            ++nextSpawnIndex;
        }

        if (nextSpawnIndex >= wave.size() && enemies.empty()) {
            roundActive = false;
            currentWave++;
            nextSpawnIndex = 0;
            spawnClock = 0.f;
        }
    }

    for (auto& enemy : enemies)
        enemy->update(deltaTime);

    if (hero)
        hero->update(deltaTime, window, enemies);

    for (auto& field : fields)
        field.update(deltaTime, enemies);

    castle.update();

    for (const auto& enemy : enemies) {
        if (enemy->reachedEnd()) {
            castle.takeDamage(50);
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const std::unique_ptr<Enemy>& e) {
                                     return e->markedForDeletion || e->reachedEnd();
                                 }), enemies.end());
}

void GameEngine::render() {
    window.clear();
    window.draw(backgroundSprite);
    castle.draw(window);

    for (auto& field : fields)
        field.draw(window);

    for (auto& enemy : enemies)
        enemy->draw(window);

    if (hero)
        hero->draw(window);
    else
        drawHeroSelectionUI();

    if (selectedField)
        shop.draw(window);

    window.draw(startButtonSprite);
    window.display();
}

void GameEngine::initHeroSelectionUI() {
    const float x = 50.f;
    const float yStart = 100.f;
    const sf::Vector2f size(150.f, 50.f);
    const float gap = 60.f;

    archerBtn.setSize(size);
    archerBtn.setPosition(x, yStart);
    archerBtn.setFillColor(sf::Color(100, 100, 200));
    archerText.setFont(uiFont);
    archerText.setString("Archer");
    archerText.setCharacterSize(20);
    archerText.setFillColor(sf::Color::White);
    archerText.setPosition(x + 10.f, yStart + 10.f);

    knightBtn.setSize(size);
    knightBtn.setPosition(x, yStart + gap);
    knightBtn.setFillColor(sf::Color(100, 100, 200));
    knightText.setFont(uiFont);
    knightText.setString("Knight");
    knightText.setCharacterSize(20);
    knightText.setFillColor(sf::Color::White);
    knightText.setPosition(x + 10.f, yStart + gap + 10.f);

    mageBtn.setSize(size);
    mageBtn.setPosition(x, yStart + 2 * gap);
    mageBtn.setFillColor(sf::Color(100, 100, 200));
    mageText.setFont(uiFont);
    mageText.setString("Mage");
    mageText.setCharacterSize(20);
    mageText.setFillColor(sf::Color::White);
    mageText.setPosition(x + 10.f, yStart + 2 * gap + 10.f);
}

void GameEngine::drawHeroSelectionUI() {
    window.draw(archerBtn);
    window.draw(knightBtn);
    window.draw(mageBtn);
    window.draw(archerText);
    window.draw(knightText);
    window.draw(mageText);
}

void GameEngine::handleHeroSelectionClick(const sf::Vector2f& mousePos) {
    if (hero) return;

    if (archerBtn.getGlobalBounds().contains(mousePos)) {
        selectedHeroType = HeroType::Archer;
    } else if (knightBtn.getGlobalBounds().contains(mousePos)) {
        selectedHeroType = HeroType::Knight;
    } else if (mageBtn.getGlobalBounds().contains(mousePos)) {
        selectedHeroType = HeroType::Mage;
    }

    const sf::Vector2f spawn = {400.f, 500.f};

    switch (selectedHeroType) {
    case HeroType::Archer:
        hero = std::make_unique<Archer>(spawn, heroTexture);
        break;
    case HeroType::Knight:
        hero = std::make_unique<Knight>(spawn, heroTexture);
        break;
    case HeroType::Mage:
        hero = std::make_unique<Mage>(spawn, heroTexture);
        break;
    default:
        break;
    }
}
