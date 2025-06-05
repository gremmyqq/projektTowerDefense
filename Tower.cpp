#include "Tower.h"
#include <cmath>
#include <memory>


Tower::Tower(const sf::Vector2f& position)
    : range(100.f), damage(10), attackCooldown(1.0f), timeSinceLastAttack(0.0f) {
    shape.setRadius(15.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
}

void Tower::update(float deltaTime) {
    timeSinceLastAttack += deltaTime;
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Tower::isInRange(const Enemy* enemy) const {
    sf::Vector2f enemyPos = enemy->getPosition();
    sf::Vector2f towerPos = shape.getPosition();
    float dx = enemyPos.x - towerPos.x;
    float dy = enemyPos.y - towerPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}

void Tower::attack(Enemy* enemy) {
    if (isInRange(enemy) && timeSinceLastAttack >= attackCooldown) {
        enemy->takeDamage(damage);
        timeSinceLastAttack = 0.0f;
    }
}
