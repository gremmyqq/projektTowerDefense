#include "TowerArcher.h"
#include <cmath>

TowerArcher::TowerArcher(const sf::Vector2f& pos)
    : TowerField(pos) {
    shape.setFillColor(sf::Color(255, 100, 0)); // kolor ognia
    damage = 25;
    range = 120.f;
    attackCooldown = 0.7f;
}

void TowerArcher::attack(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;
        float dist = std::hypot(enemy->getPosition().x - shape.getPosition().x,
                                enemy->getPosition().y - shape.getPosition().y);
        if (dist <= range && timeSinceLastAttack >= attackCooldown) {
            enemy->takeDamage(damage);
            timeSinceLastAttack = 0.f;
            break;
        }
    }
}
