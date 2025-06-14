#include "TowerField.h"
#include <cmath>

TowerField::TowerField(const sf::Vector2f& pos) {
    shape.setSize({50.f, 50.f});
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Blue);
}

void TowerField::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    timeSinceLastAttack += deltaTime;
    attackEnemies(enemies);
}

void TowerField::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool TowerField::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

void TowerField::handleClick(BuildType selectedType, GameEngine& engine) {
    // Wieża już istnieje — nic nie rób
}


void TowerField::attackEnemies(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;

        sf::Vector2f toEnemy = enemy->getPosition() - shape.getPosition();
        float distance = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

        if (distance <= range && timeSinceLastAttack >= attackCooldown) {
            enemy->takeDamage(damage);
            timeSinceLastAttack = 0.f;
            break;
        }
    }
}
