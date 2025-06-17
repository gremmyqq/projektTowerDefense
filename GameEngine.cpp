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
    castle(sf::Vector2f(280.f, 880.f)) {
    if (!startScreenTexture.loadFromFile("assets/startscreen.jpg")) {
        throw std::runtime_error("Nie można załadować startscreen.png");
    }
    startScreenSprite.setTexture(startScreenTexture);
    startScreenSprite.setScale(
        static_cast<float>(window.getSize().x) / startScreenTexture.getSize().x,
        static_cast<float>(window.getSize().y) / startScreenTexture.getSize().y
        );

    if (!victoryScreenTexture.loadFromFile("assets/winn.jpg")) {
        throw std::runtime_error("Nie można załadować winn.jpg");
    }
    victoryScreenSprite.setTexture(victoryScreenTexture);
    victoryScreenSprite.setScale(
        static_cast<float>(window.getSize().x) / victoryScreenTexture.getSize().x,
        static_cast<float>(window.getSize().y) / victoryScreenTexture.getSize().y
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
    shop.setPosition({window.getSize().x-200.f, 300.f});
    shop.setGoldPointer(&playerResources);
    // zamiast playerResources


    shop.addItem("Upgrade Castle", 100, [this]() {
        if (castle.getLevel() < castle.getMaxLevel()) {
            castle.upgrade();
            achievements.unlock(AchievementSystem::Type::CastleUpgraded);

        }
    });
    shop.addItem("Buy Knight", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Knight>(sf::Vector2f(400.f, 500.f), heroTexture);
            achievements.unlock(AchievementSystem::Type::HeroBought);

        }
    });

    shop.addItem("Buy Archer", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Archer>(sf::Vector2f(400.f, 500.f), heroTexture);
            achievements.unlock(AchievementSystem::Type::HeroBought);

        }
    });

    shop.addItem("Buy Mage", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Mage>(sf::Vector2f(400.f, 500.f), heroTexture);
            achievements.unlock(AchievementSystem::Type::HeroBought);

        }
    });
    shop.addItem("Buy Samurai", 300, [this]() {
        if (!hero) {
            hero = std::make_unique<Samurai>(sf::Vector2f(400.f, 500.f), heroTexture);
            achievements.unlock(AchievementSystem::Type::HeroBought);

        }
    });

    shop.addItem("Upgrade Hero", 200, [this]() {
        if (hero) {
            hero->upgrade();  // dodaj tę metodę do Knight/Archer/Mage
        }
    });

    buildShop.setFont(uiFont);
    buildShop.setPosition({window.getSize().x-200.f, 300.f});
    buildShop.setGoldPointer(&playerResources);

    upgradeShop.setFont(uiFont);
    upgradeShop.setPosition({window.getSize().x-200.f, 300.f});
    upgradeShop.setGoldPointer(&playerResources);


    buildShop.addItem("Build Archer", 100, [this]() {
        selectedBuildType = BuildType::TowerArcher;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Build Wizard", 150, [this]() {
        selectedBuildType = BuildType::TowerWizard;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Build Catapulte", 250, [this]() {
        selectedBuildType = BuildType::TowerCatapult;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Wood Generator", 30, [this]() {
        selectedBuildType = BuildType::GeneratorWood;

        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });


    upgradeShop.addItem("Upgrade Tower", 200, [this]() {
        if (selectedField) {
            if (auto* tower = dynamic_cast<TowerField*>(selectedField)) {
                tower->upgrade(this);
                selectedField = nullptr;
                upgradeShop.toggleVisible(false);
            }

        }
    });

    buildShop.addItem("Iron Generator", 40, [this]() {
        selectedBuildType = BuildType::GeneratorIron;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    buildShop.addItem("Food Generator", 35, [this]() {
        selectedBuildType = BuildType::GeneratorFood;
        if (selectedField) {
            selectedField->handleClick(selectedBuildType, *this);
            buildShop.toggleVisible(false);
            selectedField = nullptr;
        }
    });

    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.3f, window.getSize().y * 0.35f)));
    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.67f, window.getSize().y * 0.7f)));
    fields.emplace_back(std::make_unique<EmptyField>(sf::Vector2f(window.getSize().x * 0.42f, window.getSize().y * 0.55f)));
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
    float startX = window.getSize().x - 215.f;
    for (int i = 0; i < 3; ++i) {
        float x = startX;
        for (int j = 0; j < i; ++j) {
            x += cornerTileTextures[j].getSize().x * cornerTileSprites[j].getScale().x;
        }
        cornerTileSprites[i].setPosition(x, 150.f);
    }
    if (!coinTexture.loadFromFile("assets/2.png")) {
        throw std::runtime_error("Nie można załadować assets/2.png");
    }
    coinSprite.setTexture(coinTexture);
    coinSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));  // pierwsza klatka
    coinSprite.setScale(2.8f, 2.8f);  // powiększenie
    coinSprite.setPosition(window.getSize().x - 100.f, 165.f); // prawy górny róg
    goldText.setFont(uiFont);
    goldText.setCharacterSize(24);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(coinSprite.getPosition().x - 60.f, coinSprite.getPosition().y + 8.f);
    goldText.setString(std::to_string(playerResources));

    //Osiagniecia
    achievementsPanel.setSize({400.f, 300.f});
    achievementsPanel.setFillColor(sf::Color(0, 0, 0, 200));
    achievementsPanel.setPosition(300.f, 200.f);

    if (!achievementsButtonTexture.loadFromFile("assets/AchiButton.png")) {
        throw std::runtime_error("Nie można załadować assets/trophy.png");
    }
    achievementsButtonSprite.setTexture(achievementsButtonTexture);
    achievementsButtonSprite.setScale(4.0f, 4.0f);
    achievementsButtonSprite.setPosition(window.getSize().x - 100.f, 50.f);


    achievementsTitle.setFont(uiFont);
    achievementsTitle.setString("Achievements");
    achievementsTitle.setCharacterSize(28);
    achievementsTitle.setFillColor(sf::Color::Yellow);
    achievementsTitle.setPosition(achievementsPanel.getPosition().x + 100, achievementsPanel.getPosition().y + 10);

    // załaduj dźwięk
    if (!shootBufferArcher.loadFromFile("assets/Sounds/attackArcher.wav")) {
        throw std::runtime_error("Nie można załadować attackArcher.wav");
    }
    shootSoundsArcher.resize(8);
    for (auto& s : shootSoundsArcher)
        s.setBuffer(shootBufferArcher);

    if (!shootBufferWizard.loadFromFile("assets/Sounds/attackWizard.wav")) {
        throw std::runtime_error("Nie można załadować attackWizard.wav");
    }
    shootSoundsWizard.resize(8);
    for (auto& s : shootSoundsWizard)
        s.setBuffer(shootBufferWizard);

    if (!shootBufferCatapult.loadFromFile("assets/Sounds/attackCatapult.wav")) {
        throw std::runtime_error("Nie można załadować attackCatapult.wav");
    }
    shootSoundsCatapult.resize(8);
    for (auto& s : shootSoundsCatapult)
        s.setBuffer(shootBufferCatapult);


    if (!backgroundMusic.openFromFile("assets/Sounds/sound.wav")) {
        throw std::runtime_error("Nie można załadować assets/music.ogg");
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(40.f);  // 0–100
    backgroundMusic.play();


    // załaduj ikony dźwięku
    if (!soundButtonTextureOn.loadFromFile("assets/sound_on.png") ||
        !soundButtonTextureOff.loadFromFile("assets/sound_off.png")) {
        throw std::runtime_error("Brak ikon dźwięku");
    }

    soundButtonSprite.setTexture(soundButtonTextureOn);
    soundButtonSprite.setScale(4.f, 4.f);
    soundButtonSprite.setPosition(window.getSize().x - 200.f, 50.f);

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

            if (achievementsButtonSprite.getGlobalBounds().contains(mousePos)) {
                showAchievements = true;
                updateAchievementTexts(); // funkcja tworzy teksty na podstawie stanu osiągnięć
                return;
            }

            if (soundButtonSprite.getGlobalBounds().contains(mousePos)) {
                soundEnabled = !soundEnabled;
                soundButtonSprite.setTexture(soundEnabled ? soundButtonTextureOn : soundButtonTextureOff);

                // Wyciszenie muzyki
                backgroundMusic.setVolume(soundEnabled ? 40.f : 0.f);
                return;
            }

            // Kliknięcie w pole budowy
            bool clickedOnField = false;

            for (auto& field : fields) {
                if (field->contains(mousePos)) {
                    clickedOnField = true;

                    // Jeśli kliknięto to samo pole co poprzednio — zamknij sklep i odznacz
                    if (selectedField == field.get()) {
                        selectedField = nullptr;
                        buildShop.toggleVisible(false);
                        upgradeShop.toggleVisible(false);
                    } else {
                        selectedField = field.get();

                        // Ukryj oba sklepy na wszelki wypadek
                        buildShop.toggleVisible(false);
                        upgradeShop.toggleVisible(false);

                        if (dynamic_cast<EmptyField*>(selectedField)) {
                            //buildShop.setPosition(mousePos);
                            buildShop.toggleVisible(true);
                        } else if (dynamic_cast<TowerField*>(selectedField)) {
                            //upgradeShop.setPosition(mousePos);
                            upgradeShop.toggleVisible(true);
                        }
                    }

                    break;
                }
            }

            if (!clickedOnField) {
                selectedField = nullptr;
                buildShop.toggleVisible(false);
                upgradeShop.toggleVisible(false);
                shop.toggleVisible(false);
            }




            if (!clickedOnField) {
                selectedField = nullptr;
                upgradeShop.toggleVisible(false);
                buildShop.toggleVisible(false);
                shop.toggleVisible(false);
            }

            shop.handleClick(mousePos);
        }
        if (event.key.code == sf::Keyboard::Space && showAchievements) {
            showAchievements = false;
            return;
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
                                            if (!achievements.isUnlocked(AchievementSystem::Type::FirstKill)) {
                                            achievements.unlock(AchievementSystem::Type::FirstKill);
                                            std::cout << "[ACHIEVEMENT] Pierwszy wróg pokonany!\n";
                                         }
                                         playerResources += 30;  // 💰 złoto za zabicie
                                         return true;
                                     }

                                     if (dynamic_cast<EnemyBoss*>(e.get()) &&
                                         !achievements.isUnlocked(AchievementSystem::Type::BossKilled)) {
                                         achievements.unlock(AchievementSystem::Type::BossKilled);
                                         std::cout << "[ACHIEVEMENT] Pokonałeś bossa!\n";
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
    sf::Text text("Click left mouse button to play", uiFont, 40);
    text.setPosition(window.getSize().x/2 -300.f, window.getSize().y/2);
    text.setFillColor(sf::Color::White);
    window.draw(startScreenSprite);
    window.draw(text);
}

void GameEngine::drawVictoryScreen() {
    window.draw(victoryScreenSprite);

    sf::Text text("WYGRALES!", uiFont, 50);
    text.setPosition(400.f, 300.f);
    text.setFillColor(sf::Color::Yellow);
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
    window.draw(achievementsButtonSprite);
    window.draw(soundButtonSprite);


    if (showAchievements) {
        window.draw(achievementsPanel);
        window.draw(achievementsTitle);
        for (auto& txt : achievementTexts) {
            window.draw(txt);
        }
    }

}

void GameEngine::updateAchievementTexts() {
    achievementTexts.clear();

    std::vector<std::pair<std::string, AchievementSystem::Type>> list = {
        {"First kill", AchievementSystem::Type::FirstKill},
        {"Buy a Hero", AchievementSystem::Type::HeroBought},
        {"Upgrade castle", AchievementSystem::Type::CastleUpgraded},
        {"Upgrade Archer Tower to level 4", AchievementSystem::Type::TowerToLevel4},
        {"Defeated the Boss", AchievementSystem::Type::BossKilled}
    };

    float y = achievementsPanel.getPosition().y + 60.f;

    for (auto& [desc, type] : list) {
        sf::Text txt;
        txt.setFont(uiFont);
        txt.setCharacterSize(20);
        txt.setFillColor(achievements.isUnlocked(type) ? sf::Color::Green : sf::Color(150, 150, 150));
        txt.setString((achievements.isUnlocked(type) ? "V " : "X ") + desc);
        txt.setPosition(achievementsPanel.getPosition().x + 30.f, y);
        y += 35.f;
        achievementTexts.push_back(txt);
    }
}

void GameEngine::playArcherShootSound() {
    if (!soundEnabled) return;
    shootSoundsArcher[shootIndexArcher].stop();
    shootSoundsArcher[shootIndexArcher].play();
    shootIndexArcher = (shootIndexArcher + 1) % shootSoundsArcher.size();
}

void GameEngine::playWizardShootSound() {
    if (!soundEnabled) return;
    shootSoundsWizard[shootIndexWizard].stop();
    shootSoundsWizard[shootIndexWizard].play();
    shootIndexWizard = (shootIndexWizard + 1) % shootSoundsWizard.size();
}

void GameEngine::playCatapultShootSound() {
    if (!soundEnabled) return;
    shootSoundsCatapult[shootIndexCatapult].stop();
    shootSoundsCatapult[shootIndexCatapult].play();
    shootIndexCatapult = (shootIndexCatapult + 1) % shootSoundsCatapult.size();
}
