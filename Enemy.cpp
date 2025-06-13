//
// Created by CP on 30.05.2025.
//
#include "Enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(const std::vector<sf::Vector2f>& path)
    : path(path), currentTargetIndex(0) {
    if (!path.empty()) {
        position = path[0];
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    std::cout << "Enemy took damage! New HP: " << health << std::endl;
    if (health < 0) health = 0;
}

bool Enemy::isDead() const {
    return health <= 0;
}

bool Enemy::reachedEnd() const {
    return currentTargetIndex >= path.size();
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(sf::Vector2f newPos) {
    position = newPos;
}

void Enemy::moveTowardsTarget(float deltaTime) {
    if (currentTargetIndex >= path.size()) return;

    sf::Vector2f target = path[currentTargetIndex];
    sf::Vector2f direction = target - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f) {
        ++currentTargetIndex;
    } else {
        direction /= distance;
        position += direction * speed * deltaTime;
    }
}
