//
// Created by CP on 30.05.2025.
//

#include "EnemyRegular.h"

EnemyRegular::EnemyRegular(const std::vector<sf::Vector2f>& path)
    : Enemy(path) // speed, health
{
    maxHealth=100;
    health=maxHealth;
    speed=10;
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));

}

void EnemyRegular::update(float deltaTime) {
    if (!reachedEnd() && !isDead()) {
        moveTowardsTarget(deltaTime);
    }
}

void EnemyRegular::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}
