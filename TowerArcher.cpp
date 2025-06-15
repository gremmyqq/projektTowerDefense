#include "TowerArcher.h"
#include <cmath>
#include <iostream>

TowerArcher::TowerArcher(const sf::Vector2f& pos)
    : TowerField(pos)
{
    if (!texture.loadFromFile("assets/archer_all_levels.png")) {
        std::cerr << "Nie można załadować archer_all_levels.png\n";
    }

    sprite.setTexture(texture);
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(pos);

    shape.setFillColor(sf::Color::Transparent);
    sprite.setTextureRect(sf::IntRect(4 * frameWidth, (level - 1) * frameHeight, frameWidth, frameHeight));
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
            upgradeTime = 0.f;
            currentFrame = 4;
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
    if (animTimer >= animInterval) {
        animTimer = 0.f;

        int row = level - 1;
        int startFrame = (currentAnim == AnimationType::Upgrade) ? 0 : 4;
        int maxFrame = startFrame + 4;

        currentFrame++;
        if (currentFrame >= maxFrame) {
            currentFrame = startFrame;
        }

        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight));
    }
}

void TowerArcher::upgrade() {
    if (level < maxLevel) {
        ++level;
        currentAnim = AnimationType::Upgrade;
        isUpgrading = true;
        upgradeTime = 0.f;
        currentFrame = 0;
    }
}

bool TowerArcher::contains(const sf::Vector2f& point) const {
    return sprite.getGlobalBounds().contains(point);
}

