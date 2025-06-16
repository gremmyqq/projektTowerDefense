#ifndef SPELL_H
#define SPELL_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Spell {
public:
    Spell(const sf::Vector2f& startPos, const sf::Vector2f& targetPos, const sf::Texture& texture);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkCollision(Enemy& enemy);
    bool isOffScreen(const sf::RenderWindow& window) const;
    void markForRemoval();
    bool isMarked() const;

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = 400.f;
    bool markedForRemoval = false;
};

#endif
