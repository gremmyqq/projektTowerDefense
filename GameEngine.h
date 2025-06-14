#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "EnemyRegular.h"
#include "EnemyFast.h"
#include "EnemyTank.h"
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

class GameEngine {
public:
    explicit GameEngine(sf::RenderWindow& window);
    void run();

    void setSelectedBuildType(BuildType type) {
        selectedBuildType = type;
    }
    void replaceField(Field* oldField, std::unique_ptr<Field> newField);

private:
    sf::RenderWindow& window;

    // Komponenty gry
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Field>> fields;
    std::unique_ptr<HeroBase> hero;
    Field* selectedField = nullptr;
    Shop shop;
    sf::Font uiFont;
    int playerResources = 100;
    float spawnTimer;
    float spawnInterval;
    Castle castle;

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




    sf::RectangleShape archerBtn, knightBtn, mageBtn;
    sf::Text archerText, knightText, mageText;

    // Metody pomocnicze
    void handleEvents();
    void update(float deltaTime);
    void render();
    void initHeroSelectionUI();
    void drawHeroSelectionUI();
    void handleHeroSelectionClick(const sf::Vector2f& mousePos);
    //void spawnEnemy();
};

#endif // GAMEENGINE_H
