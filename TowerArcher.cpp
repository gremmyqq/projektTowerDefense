#include "TowerArcher.h"
#include <cmath>
#include <iostream>

TowerArcher::TowerArcher(const sf::Vector2f& pos)
    : TowerField(pos)
{
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent);

    loadAnimation(AnimationType::Idle);
}

void TowerArcher::attack(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;

        float dist = std::hypot(
            enemy->getPosition().x - shape.getPosition().x,
            enemy->getPosition().y - shape.getPosition().y
            );

        if (dist <= range && timeSinceLastAttack >= attackCooldown) {
            enemy->takeDamage(damage);
            timeSinceLastAttack = 0.f;
            break;
        }
    }
}

void TowerArcher::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    timeSinceLastAttack += deltaTime;

    if (isUpgrading) {
        upgradeTime += deltaTime;
        if (upgradeTime >= upgradeDuration) {
            isUpgrading = false;
            currentAnim = AnimationType::Idle;
            loadAnimation(currentAnim);
        }
    }

    attack(enemies);
    updateAnimation(deltaTime);
}

void TowerArcher::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void TowerArcher::updateAnimation(float deltaTime) {
    animTimer += deltaTime;
    if (animTimer < animInterval) return;
    animTimer = 0.f;

    currentFrame = (currentFrame + 1) % frameCount;
    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
}

void TowerArcher::loadAnimation(AnimationType type) {
    std::string path;
    if (type == AnimationType::Idle) {
        path = "assets/ArcherTower/2 Idle/" + std::to_string(level) + ".png";
    } else {
        path = "assets/ArcherTower/1 Upgrade/" + std::to_string(level) + ".png";
    }

    if (!idleTexture.loadFromFile(path)) {
        std::cerr << "[BŁĄD] Nie można załadować tekstury: " << path << "\n";
        return;
    }

    sprite.setTexture(idleTexture);
    frameCount = (type == AnimationType::Idle) ? 4 : 4; // zmień jeśli potrzeba
    currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void TowerArcher::upgrade() {
    if (level < maxLevel) {
        ++level;
        currentAnim = AnimationType::Upgrade;
        isUpgrading = true;
        upgradeTime = 0.f;
        loadAnimation(AnimationType::Upgrade);
    } else {
        std::cout << "[INFO] Maksymalny poziom osiągnięty\n";
    }
}

bool TowerArcher::contains(const sf::Vector2f& point) const {
    return sprite.getGlobalBounds().contains(point);
}
