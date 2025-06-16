//
// Created by CP on 30.05.2025.
//
#include "Enemy.h"
#include <cmath>
#include "HeroBase.h"  // dodaj na górze jeśli brak

#include <cstdlib> // dla std::rand()
#include <ctime>   // dla std::time()

Enemy::Enemy(const std::vector<sf::Vector2f>& originalPath)
    : currentTargetIndex(0) {

    // Jednorazowa inicjalizacja generatora
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    // Losowe przesunięcie każdego waypointa
    for (const auto& point : originalPath) {
        float offsetX = static_cast<float>(std::rand() % 31 - 15); // -15 do +15
        float offsetY = static_cast<float>(std::rand() % 31 - 15); // -15 do +15
        path.emplace_back(point.x + offsetX, point.y + offsetY);
    }

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
    if (state == EnemyState::Attacking || state == EnemyState::Dying) return; // 🚫 Nie ruszaj się w tych stanach

    if (currentTargetIndex >= path.size()) return;

    sf::Vector2f target = path[currentTargetIndex];
    sf::Vector2f direction = target - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f) {
        ++currentTargetIndex;
    } else {
        direction /= distance;
        updateSpriteDirection(direction); // tylko w stanie Walking
        position += direction * speed * deltaTime;
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
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                onAttackAnimationFinished();  // ← wywołanie wirtualne
                state = EnemyState::Walking;
                currentFrame = 0;
            }

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
    if (canAttack==true) {
        state = EnemyState::Attacking;
        currentFrame = 0;
        animationTimer = 0.f;
    }

}

void Enemy::die() {
    state = EnemyState::Dying;
    currentFrame = 0;
    animationTimer = 0.f;
}




void Enemy::updateAgainstHero(float deltaTime, HeroBase& hero) {
    if (state == EnemyState::Dying) return;

    attackTimer += deltaTime;

    sf::Vector2f toHero = hero.getPosition() - position;
    float distance = std::sqrt(toHero.x * toHero.x + toHero.y * toHero.y);

    if (distance <= attackRange) {
        updateSpriteDirection(toHero);

        if (attackTimer >= attackCooldown) {
            startAttack();
            hero.takeDamage(attackDamage);
            attackTimer = 0.f;
        }
    }
}
