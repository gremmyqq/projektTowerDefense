#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Tower {
public:
    Tower(const sf::Vector2f& position);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void attack(Enemy* enemy);

    bool isInRange(const Enemy* enemy) const;

private:
    sf::CircleShape shape;
    float range;
    int damage;
    float attackCooldown;
    float timeSinceLastAttack;
};

#endif // TOWER_H
