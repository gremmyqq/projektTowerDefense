#include "GameEngine.h"
#include <stdexcept>
#include "Archer.h"
#include "EnemyArcher.h"
#include "Knight.h"
#include "Mage.h"
#include "Samurai.h"


GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine(sf::RenderWindow& window)
    : window(window),
    spawnTimer(0.f),
    spawnInterval(3.f),
    castle(sf::Vector2f(280.f, 1080.f)) {
    if (!startScreenTexture.loadFromFile("assets/startscreen.jpg")) {
        throw std::runtime_error("Nie można załadować startscreen.png");
    }
    startScreenSprite.setTexture(startScreenTexture);
    startScreenSprite.setScale(
        static_cast<float>(window.getSize().x) / startScreenTexture.getSize().x,
        static_cast<float>(window.getSize().y) / startScreenTexture.getSize().y
        );

    if (!defeatScreenTexture.loadFromFile("assets/gameove.jpg")) {
        throw std::runtime_error("Nie można załadować gameover.png");
    }
    defeatScreenSprite.setTexture(defeatScreenTexture);
    defeatScreenSprite.setScale(
        static_cast<float>(window.getSize().x) / defeatScreenTexture.getSize().x,
        static_cast<float>(window.getSize().y) / defeatScreenTexture.getSize().y
        );
    GameEngine::instance = this;
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

    buildShop.setFont(uiFont);
    buildShop.setPosition({300.f, 500.f});
    buildShop.setGoldPointer(&playerResources);

    upgradeShop.setFont(uiFont);
    upgradeShop.setPosition({300.f, 500.f});
    upgradeShop.setGoldPointer(&playerResources);


    buildShop.addItem("Postaw Łucznika", 100, [this]() {
        selectedBuildType = BuildType::TowerArcher;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Postaw Wizard", 150, [this]() {
        selectedBuildType = BuildType::TowerWizard;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Postaw Katapulte", 250, [this]() {
        selectedBuildType = BuildType::TowerCatapult;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Generator Drewna", 30, [this]() {
        selectedBuildType = BuildType::GeneratorWood;

        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });


    upgradeShop.addItem("Ulepsz wieżę", 200, [this]() {
        if (selectedField) {
            if (auto* tower = dynamic_cast<TowerField*>(selectedField)) {
                tower->upgrade();
                selectedField = nullptr;
                upgradeShop.toggleVisible(false);
            }

        }
    });

    buildShop.addItem("Generator Żelaza", 40, [this]() {
        selectedBuildType = BuildType::GeneratorIron;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Generator Żywności", 35, [this]() {
        selectedBuildType = BuildType::GeneratorFood;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.3f, window.getSize().y * 0.3f)));
    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.7f, window.getSize().y * 0.7f)));
    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y * 0.5f)));
    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.4f, window.getSize().y * 0.2f)));
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
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }
        if (currentState == GameState::StartScreen) {
            if (event.type == sf::Event::MouseButtonPressed) {
                currentState = GameState::Playing;
            }
            return;
        }


        if (currentState == GameState::Victory || currentState == GameState::Defeat) {
            if (event.type == sf::Event::MouseButtonPressed) {
                //restartGame();  // napisz tę funkcję jeśli chcesz restartować
            }
            return;
        }

        // 1. Zamknięcie okna / ESC


        // 2. Klawiatura – Q i E dla bohaterów
        if (event.type == sf::Event::KeyPressed) {
            if (hero) {
                if (auto* mage = dynamic_cast<Mage*>(hero.get())) {
                    if (event.key.code == sf::Keyboard::Q)
                        mage->queueAttack();
                    if (event.key.code == sf::Keyboard::E)
                        mage->queueExtraAttack();
                }

                if (auto* samurai = dynamic_cast<Samurai*>(hero.get())) {
                    if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::E)
                        samurai->queueAttack(event.key.code);
                }

                if (auto* knight = dynamic_cast<Knight*>(hero.get())) {
                    if (event.key.code == sf::Keyboard::Q)
                        knight->queueAttack();
                    // E ignorujemy – brak reakcji
                }
                if (auto* archer = dynamic_cast<Archer*>(hero.get())) {
                    if (event.key.code == sf::Keyboard::Q)
                        archer->queueAttack();
                    // E ignorujemy
                }

            }
        }


        // 3. Obsługa myszki – tylko przyciski interfejsu
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            // Przycisk START
            if (startButtonSprite.getGlobalBounds().contains(mousePos) && !roundActive) {
                roundActive = true;
                spawnClock = 0;
                if (shop.isVisible()) {
                    shop.toggleVisible();
                }
                return;
            }

            // Przycisk SKLEPU
            if (shopButtonSprite.getGlobalBounds().contains(mousePos)) {
                shop.toggleVisible();
                return;
            }

            // Kliknięcie w sklep
            if (shop.isVisible()) {
                shop.handleClick(mousePos);
                return;
            }

            if (buildShop.isVisible()) {
                buildShop.handleClick(mousePos);
                return;
            }
            if (upgradeShop.isVisible()) {
                upgradeShop.handleClick(mousePos);
                return;
            }


            // Kliknięcie w pole budowy
            bool clickedOnField = false;
            upgradeShop.toggleVisible(false);
            buildShop.toggleVisible(false);

            for (auto& field : fields) {
                if (field->contains(mousePos)) {
                    selectedField = field.get();
                    clickedOnField = true;


                    upgradeShop.toggleVisible(false);
                    buildShop.toggleVisible(false);

                    if (dynamic_cast<EmptyField*>(selectedField)) {
                        buildShop.toggleVisible(true);
                        upgradeShop.toggleVisible(false);
                    } else if (dynamic_cast<TowerField*>(selectedField)) {
                        buildShop.toggleVisible(false);
                        upgradeShop.toggleVisible(true);
                    } else {
                        buildShop.toggleVisible(false);
                        upgradeShop.toggleVisible(false);
                    }


                    break;
                }
            }

            if (!clickedOnField) {
                selectedField = nullptr;
                upgradeShop.toggleVisible(false);
                buildShop.toggleVisible(false);
                shop.toggleVisible(false);
            }

            shop.handleClick(mousePos);
        }
    }
}


void GameEngine::update(float deltaTime) {
    spawnClock += deltaTime;
    //std::cout << "[DEBUG] Liczba pól: " << fields.size() << "\n";
    if (currentState != GameState::Playing) return;

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
                }else if (spawn.type == " Boss") {
                    enemies.emplace_back(std::make_unique<EnemyBoss>(allPaths[spawn.pathIndex]));
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
                                 [this](const std::unique_ptr<Enemy>& e) {
                                     if (e->markedForDeletion) {
                                         playerResources += 30;  // 💰 złoto za zabicie
                                         return true;
                                     }
                                     return e->reachedEnd();
                                 }), enemies.end());
    if (castle.getHP() <= 0 && currentState == GameState::Playing) {
        currentState = GameState::Defeat;
    }

    if (currentWave >= waveMap.size() && enemies.empty() && currentState == GameState::Playing) {
        currentState = GameState::Victory;
    }


}

void GameEngine::render() {
    window.clear();

    switch (currentState) {
    case GameState::StartScreen:
        drawStartScreen();
        break;

    case GameState::Playing:
        drawGame();  // przesuń tam aktualny kod renderowania gry
        break;

    case GameState::Victory:
        drawVictoryScreen();
        break;

    case GameState::Defeat:
        drawDefeatScreen();
        break;
    }

    window.display();
}





void GameEngine::replaceField(Field* oldField, std::unique_ptr<Field> newField) {
    auto it = std::find_if(fields.begin(), fields.end(),
                           [oldField](const std::unique_ptr<Field>& f) { return f.get() == oldField; });

    if (it != fields.end()) {
        *it = std::move(newField);
    }
}
void GameEngine::drawStartScreen() {
    sf::Text text("Kliknij lewym przyciskiem myszy aby rozpoczac", uiFont, 40);
    text.setPosition(300.f, 300.f);
    text.setFillColor(sf::Color::White);
    window.draw(startScreenSprite);
    window.draw(text);
}

void GameEngine::drawVictoryScreen() {
    sf::Text text("Wygrałeś!", uiFont, 50);
    text.setPosition(400.f, 300.f);
    text.setFillColor(sf::Color::Green);
    window.draw(text);
}

void GameEngine::drawDefeatScreen() {
    sf::Text text("Game over, kristofer!", uiFont, 50);
    text.setPosition(400.f, 300.f);
    text.setFillColor(sf::Color::Red);
    window.draw(defeatScreenSprite);
    window.draw(text);
}
void GameEngine::drawGame() {
    window.draw(backgroundSprite);
    castle.draw(window);

    for (auto& field : fields)
        field->draw(window);

    for (auto& enemy : enemies)
        enemy->draw(window);

    if (hero)
        hero->draw(window);

    if (selectedField)
        shop.draw(window);

    if (shop.isVisible())
        shop.draw(window);

    for (int i = 0; i < 3; ++i) {
        window.draw(cornerTileSprites[i]);
    }

    if (buildShop.isVisible()) {
        buildShop.draw(window);
    }

    if (upgradeShop.isVisible()) {
        upgradeShop.draw(window);
    }

    window.draw(coinSprite);
    window.draw(goldText);
    window.draw(shopButtonSprite);
    window.draw(startButtonSprite);
}

