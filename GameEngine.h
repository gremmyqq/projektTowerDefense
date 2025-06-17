#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "EnemyRegular.h"
#include "EnemyFast.h"
#include "EnemyTank.h"
#include "EnemyBoss.h"
#include "Shop.h"
#include "Field.h"
#include "HeroBase.h"
#include "Archer.h"
#include "Knight.h"
#include "Mage.h"
#include "Castle.h"
#include "LevelLoader.h"
#include "EmptyField.h"
#include "TowerField.h"
#include "GeneratorField.h"
#include "TowerArcher.h"
#include "TowerWizard.h"
enum class GameState {
    StartScreen,
    HeroSelection,
    Playing,
    RoundTransition,
    Victory,
    Defeat
};

class GameEngine {
public:
    explicit GameEngine(sf::RenderWindow& window);
    void run();

    void setSelectedBuildType(BuildType type) {
        selectedBuildType = type;
    }
    void replaceField(Field* oldField, std::unique_ptr<Field> newField);

    static GameEngine* instance;  // ← dodaj to
    Castle& getCastle() { return castle; }  // ← getter

    int* getPlayerResourcesPointer() { return &playerResources; }

private:
    sf::RenderWindow& window;

    // Komponenty gry
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Field>> fields;
    std::unique_ptr<HeroBase> hero;
    Field* selectedField = nullptr;
    Shop shop;
    Shop buildShop;
    Shop upgradeShop;


    sf::Font uiFont;
    int playerResources = 1000;
    int playerGold = 500;

    float spawnTimer;
    float spawnInterval;
    Castle castle;
    bool shopVisible = false; // Dodaj do prywatnych pól klasy GameEngine

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

    //przycisk start rundy
    sf::Texture startButtonTexture;
    sf::Sprite startButtonSprite;

    sf::Texture shopButtonTexture;
    sf::Sprite shopButtonSprite;
    bool roundActive = false;

    // Ścieżka dla wrogów
    std::vector<sf::Vector2f> path;

    // Tekstury
    sf::Texture heroTexture;

    // Wybór bohatera
    enum class HeroType {
        None,
        Archer,
        Knight,
        Mage
    };
    HeroType selectedHeroType = HeroType::None;

    BuildType selectedBuildType = BuildType::None;
    GameState currentState = GameState::StartScreen;





sf::RectangleShape archerBtn, knightBtn, mageBtn;
    sf::Text archerText, knightText, mageText;

    // Metody pomocnicze
    void handleEvents();
    void update(float deltaTime);
    void render();
    void handleHeroSelectionClick(const sf::Vector2f& mousePos);
    void spawnEnemy();
    sf::Texture cornerTileTextures[3];
    sf::Sprite cornerTileSprites[3];
    sf::Texture coinTexture;
    sf::Sprite coinSprite;
    int coinFrame = 0;
    float coinFrameTime = 0.f;
    const int coinMaxFrames = 6;
    const float coinFrameDuration = 0.1f;  // czas trwania jednej klatki
    sf::Text goldText;
    void drawStartScreen();
    void drawVictoryScreen();
    void drawDefeatScreen();
    void drawGame();
    sf::Texture startScreenTexture;
    sf::Sprite startScreenSprite;
    sf::Texture victoryScreenTexture;
    sf::Sprite victoryScreenSprite;
    sf::Texture defeatScreenTexture;
    sf::Sprite defeatScreenSprite;

};

#endif // GAMEENGINE_H
