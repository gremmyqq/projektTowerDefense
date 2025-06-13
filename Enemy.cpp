//
// Created by CP on 30.05.2025.
//
#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::Vector2f>& path)
    : path(path), currentTargetIndex(0) {
    if (!path.empty()) {
        position = path[0];
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0 && state != EnemyState::Dying) {
        die();
    }
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
    sf::Vector2f directionVec = target - position;
    float distance = std::sqrt(directionVec.x * directionVec.x + directionVec.y * directionVec.y);

    if (distance < 1.0f) {
        ++currentTargetIndex;
    } else {
        directionVec /= distance;
        updateSpriteDirection(directionVec); // nowa linia
        position += directionVec * speed * deltaTime;
    }
}

void Enemy::updateSpriteDirection(const sf::Vector2f& dir) {
    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;

    if (angle >= -45 && angle <= 45) direction = 2;         // prawo
    else if (angle > 45 && angle < 135) direction = 0;      // dół
    else if (angle >= 135 || angle <= -135) direction = 2;  // lewo
    else direction = 1;                                     // góra
}

void Enemy::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= animationInterval) {
        animationTimer = 0.f;

        if (state == EnemyState::Dying) {
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                markedForDeletion = true; // gotowy do usunięcia
            }
        } else if (state == EnemyState::Attacking) {
            currentFrame = (currentFrame + 1) % totalFrames;
            // Możesz dodać: if (currentFrame == 0) state = EnemyState::Walking;
        } else {
            currentFrame = (currentFrame + 1) % totalFrames;
        }
    }

    int row = 0;
    switch (state) {
    case EnemyState::Walking: row = direction+6; break;           // 0–3
    case EnemyState::Attacking: row = direction; break;                 // atak — np. 4. wiersz
    case EnemyState::Dying: row = direction+3; break;                     // śmierć — np. 5. wiersz
    }

    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight));
    sprite.setPosition(position);
}


void Enemy::startAttack() {
    state = EnemyState::Attacking;
    currentFrame = 0;
    animationTimer = 0.f;
}

void Enemy::die() {
    state = EnemyState::Dying;
    currentFrame = 0;
    animationTimer = 0.f;
}


