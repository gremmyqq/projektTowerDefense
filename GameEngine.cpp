#include "GameEngine.h"
#include <stdexcept>
#include "Archer.h"
#include "EnemyArcher.h"
#include "Knight.h"
#include "Mage.h"
#include "Samurai.h"

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
    startButtonSprite.setPosition(window.getSize().x - 100.f, window.getSize().y - 150.f);

    if (!shopButtonTexture.loadFromFile("assets/shop.png")) {
        throw std::runtime_error("Nie można załadować start_button.bmp");
    }
    shopButtonSprite.setTexture(shopButtonTexture);
    shopButtonSprite.setScale(0.09f, 0.09f);
    shopButtonSprite.setOrigin(
        shopButtonTexture.getSize().x / 2.f,
        shopButtonTexture.getSize().y / 2.f

        );
    shopButtonSprite.setPosition(window.getSize().x - 100.f, window.getSize().y - 225.f);
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
  // zamiast playerResources


    shop.addItem("Ulepsz zamek", 100, [this]() {
        if (castle.getLevel() < castle.getMaxLevel()) {
            castle.upgrade();
        }
    });
    shop.addItem("Kup Rycerza", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Knight>(sf::Vector2f(400.f, 500.f), heroTexture);
        }
    });

    shop.addItem("Kup Łucznika", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Archer>(sf::Vector2f(400.f, 500.f), heroTexture);
        }
    });

    shop.addItem("Kup Maga", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Mage>(sf::Vector2f(400.f, 500.f), heroTexture);
        }
    });
    shop.addItem("Kup Samuraia", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Samurai>(sf::Vector2f(400.f, 500.f), heroTexture);
        }
    });

    shop.addItem("Ulepsz bohatera", 200, [this]() {
        if (hero) {
            hero->upgrade();  // dodaj tę metodę do Knight/Archer/Mage
        }
    });

    towerShop.setFont(uiFont);
    towerShop.setPosition({300.f, 500.f});
    towerShop.setGoldPointer(&playerResources);

    // Dla pustego pola
    towerShop.addItem("Postaw Łucznika", 50, [this]() {
        selectedBuildType = BuildType::TowerArcher;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            towerShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });


    towerShop.addItem("Postaw Generator", 30, [this]() {
        selectedBuildType = BuildType::Generator;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            towerShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });


    // Dla istniejącej wieży
    towerShop.addItem("Ulepsz wieżę", 100, [this]() {
        if (selectedField) {
            if (auto* archer = dynamic_cast<TowerArcher*>(selectedField)) {
                archer->upgrade();
                selectedField = nullptr;
                towerShop.toggleVisible(false);

            }
        }
    });


    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.3f, window.getSize().y * 0.3f)));
    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.7f, window.getSize().y * 0.7f)));

    //initHeroSelectionUI();
    const std::string tilePaths[3] = {
        "assets/Tile_01.png",
        "assets/Tile_02.png",
        "assets/Tile_03.png"
    };

    float totalWidth = 50.f;

    for (int i = 0; i < 3; ++i) {
        if (!cornerTileTextures[i].loadFromFile(tilePaths[i])) {
            throw std::runtime_error("Nie można załadować " + tilePaths[i]);
        }
        cornerTileSprites[i].setTexture(cornerTileTextures[i]);

        // Opcjonalnie skalowanie (np. zmniejsz o połowę)
        cornerTileSprites[i].setScale(4.f, 5.f);

        totalWidth += cornerTileTextures[i].getSize().x * cornerTileSprites[i].getScale().x;
    }

    // Ustaw pozycje tak, by kafelki były od prawej krawędzi do lewej
    float startX = window.getSize().x - 600.f;
    for (int i = 0; i < 3; ++i) {
        float x = startX;
        for (int j = 0; j < i; ++j) {
            x += cornerTileTextures[j].getSize().x * cornerTileSprites[j].getScale().x;
        }
        cornerTileSprites[i].setPosition(x, 28.f);
    }
    if (!coinTexture.loadFromFile("assets/2.png")) {
        throw std::runtime_error("Nie można załadować assets/2.png");
    }
    coinSprite.setTexture(coinTexture);
    coinSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));  // pierwsza klatka
    coinSprite.setScale(2.8f, 2.8f);  // powiększenie
    coinSprite.setPosition(window.getSize().x - 470.f, 44.f); // prawy górny róg
    goldText.setFont(uiFont);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(coinSprite.getPosition().x - 60.f, coinSprite.getPosition().y + 8.f);
    goldText.setString(std::to_string(playerResources));

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
        // 1. Zamknięcie okna / ESC
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }

        // 2. Klawiatura – Q i E dla Samuraia
        if (event.type == sf::Event::KeyPressed) {
            if (hero) {
                Samurai* samurai = dynamic_cast<Samurai*>(hero.get());
                if (samurai && (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::E)) {
                    samurai->queueAttack(event.key.code);
                }
            }
        }

        // 3. Myszka – LPM i PPM do standardowego ataku
        if (event.type == sf::Event::MouseButtonPressed) {
            if (hero) {
                if (event.mouseButton.button == sf::Mouse::Left)
                    hero->queueAttack();
                if (event.mouseButton.button == sf::Mouse::Right)
                    hero->queueExtraAttack();
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            // 4. Przycisk START
            if (startButtonSprite.getGlobalBounds().contains(mousePos) && !roundActive) {
                roundActive = true;
                spawnClock = 0;
                if (shop.isVisible()) {
                    shop.toggleVisible();
                }
                return;
            }

            // 5. Przycisk SKLEPU
            if (shopButtonSprite.getGlobalBounds().contains(mousePos)) {
                shop.toggleVisible();
                return;
            }

            // 6. Kliknięcie w sam sklep
            if (shop.isVisible()) {
                shop.handleClick(mousePos);
                return;
            }

            if (towerShop.isVisible()) {
                towerShop.handleClick(mousePos);
                return;
            }

            // 7. Kliknięcie w pole
            if (!hero) return;

            bool clickedOnField = false;
            towerShop.toggleVisible(false);

            for (auto& field : fields) {
                if (field->contains(mousePos)) {
                    selectedField = field.get();
                    clickedOnField = true;

                    if (dynamic_cast<EmptyField*>(selectedField)) {
                        towerShop.toggleVisible(true);
                    } else if (dynamic_cast<TowerField*>(selectedField)) {
                        towerShop.toggleVisible(true);
                    } else {
                        towerShop.toggleVisible(false);
                    }
                    break;
                }
            }

            if (!clickedOnField) {
                selectedField = nullptr;
                towerShop.toggleVisible(false);
            }

            shop.handleClick(mousePos);
        }
    }
}


void GameEngine::update(float deltaTime) {
    spawnClock += deltaTime;
    //std::cout << "[DEBUG] Liczba pól: " << fields.size() << "\n";

    if (roundActive && waveMap.find(currentWave) != waveMap.end()) {
        const auto& wave = waveMap[currentWave];

        if (nextSpawnIndex < wave.size() &&
            spawnClock >= wave[nextSpawnIndex].delay) {

            const auto& spawn = wave[nextSpawnIndex];
            if (spawn.pathIndex >= 0 && spawn.pathIndex < static_cast<int>(allPaths.size())) {
                if (spawn.type == " Regular") {
                    enemies.emplace_back(std::make_unique<EnemyRegular>(allPaths[spawn.pathIndex]));
                } else if (spawn.type == " Fast") {
                    enemies.emplace_back(std::make_unique<EnemyFast>(allPaths[spawn.pathIndex]));
                } else if (spawn.type == " Tank") {
                    enemies.emplace_back(std::make_unique<EnemyTank>(allPaths[spawn.pathIndex]));
                }else if (spawn.type == " Archer") {
                    enemies.emplace_back(std::make_unique<EnemyArcher>(allPaths[spawn.pathIndex]));
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

    if (hero && !hero->isDead()) {
        for (auto& enemy : enemies) {
            enemy->updateAgainstHero(deltaTime, *hero);
        }
    }


    for (auto& field : fields)
        field->update(deltaTime, enemies);

    castle.update();

    for (const auto& enemy : enemies) {
        if (enemy->reachedEnd()) {
            castle.takeDamage(50);
        }
    }
    coinFrameTime += deltaTime;
    if (coinFrameTime >= coinFrameDuration) {
        coinFrame = (coinFrame + 1) % coinMaxFrames;
        coinSprite.setTextureRect(sf::IntRect(coinFrame * 16, 0, 16, 16));
        coinFrameTime = 0.f;
    }
    goldText.setString(std::to_string(playerResources));
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
        field->draw(window);

    for (auto& enemy : enemies)
        enemy->draw(window);

    if (hero)
        hero->draw(window);
    else
        //drawHeroSelectionUI();

    if (selectedField)
        shop.draw(window);
    if (shop.isVisible())
        shop.draw(window);

    for (int i = 0; i < 3; ++i) {
        window.draw(cornerTileSprites[i]);
    }

    if (towerShop.isVisible()) {
        towerShop.draw(window);
    }


    window.draw(coinSprite);
    window.draw(goldText);
    window.draw(shopButtonSprite);
    window.draw(startButtonSprite);
    window.display();
}




void GameEngine::replaceField(Field* oldField, std::unique_ptr<Field> newField) {
    auto it = std::find_if(fields.begin(), fields.end(),
                           [oldField](const std::unique_ptr<Field>& f) { return f.get() == oldField; });

    if (it != fields.end()) {
        *it = std::move(newField);
    }
}

