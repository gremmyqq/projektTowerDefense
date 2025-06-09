//
// Created by CP on 30.05.2025.
//

#include "EnemyRegular.h"

EnemyRegular::EnemyRegular(const std::vector<sf::Vector2f>& path)
    : Enemy(path) {
    maxHealth = 100;
    health = maxHealth;
    speed = 100;
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));

    if (!path.empty()) {
        setPosition(path[0]);  // <- jawne ustawienie pozycji
    }
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
