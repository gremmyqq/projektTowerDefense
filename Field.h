#ifndef FIELD_H
#define FIELD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"

enum class FieldType {
    Empty,
    Tower,
    Generator
};

class Field {
public:
    Field(const sf::Vector2f& position);

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);

    void handleClick(const sf::Vector2f& mousePos); // kliknięcie myszką
    bool contains(const sf::Vector2f& point) const;

private:
    sf::RectangleShape shape;
    FieldType type;

    static sf::Texture fieldTexture; // ← współdzielona
    sf::Sprite fieldSprite;

    // --- tylko dla wieży
    float attackCooldown;
    float timeSinceLastAttack;
    float range;
    int damage;

    // --- tylko dla generatora
    float resourceTimer;
    float resourceInterval;
    int* resourceCount; // wskaźnik na licznik surowców

    void buildTower();
    void buildGenerator();

    void attackEnemies(std::vector<std::unique_ptr<Enemy>>& enemies);
    void generateResources();
};

#endif
