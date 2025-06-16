#include "TowerCatapult.h"
#include <cmath>
#include <iostream>

TowerCatapult::TowerCatapult(const sf::Vector2f& pos)
    : TowerField(pos)
{
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setScale(1.25f, 1.25f);
    sprite.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent);

    loadAnimation(AnimationType::Idle);
    if (!arrowTexture.loadFromFile("assets/WizardTower/FB500-1.png")) {
        std::cerr << "[BŁĄD] Nie można załadować strzały!\n";
    }

}

void TowerCatapult::attack(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;

        float dist = std::hypot(
            enemy->getPosition().x - shape.getPosition().x,
            enemy->getPosition().y - shape.getPosition().y
            );
        //std::cout<<dist<<"\n";

        if (dist <= range && timeSinceLastAttack >= attackCooldown) {
            arrows.emplace_back(shape.getPosition(), enemy->getPosition(), arrowTexture, 800.f);

            std::cout<<"strzal\n";
            timeSinceLastAttack = 0.f;
            break;
        }
    }
}

void TowerCatapult::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    timeSinceLastAttack += deltaTime;

    if (isUpgrading) {
        upgradeTime += deltaTime;
        if (upgradeTime >= upgradeDuration) {
            isUpgrading = false;
            currentAnim = AnimationType::Idle;
            loadAnimation(AnimationType::Idle);  // załaduj nowe idle z aktualnym poziomem
        }

    }

    attack(enemies);
    updateAnimation(deltaTime);

    for (auto& arrow : arrows) {
        arrow.update(deltaTime);
    }

    arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
                                [&](Arrow& arrow) {
                                    if (arrow.isMarked()) return true;

                                    for (auto& enemy : enemies) {
                                        if (!enemy->isDead() && arrow.checkCollision(*enemy)) {
                                            enemy->takeDamage(damage);
                                            arrow.markForRemoval();
                                            return true;
                                        }
                                    }
                                    return false;
                                }), arrows.end());

}

void TowerCatapult::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (auto& arrow : arrows) {
        arrow.draw(window);
    }

}

void TowerCatapult::updateAnimation(float deltaTime) {
    if (frameCount <= 1) return; // ❗ NIE animuj jeśli tylko jedna klatka

    animTimer += deltaTime;
    if (animTimer < animInterval) return;
    animTimer = 0.f;

    currentFrame++;

    if (currentAnim == AnimationType::Upgrade) {
        if (currentFrame >= frameCount) {
            // zakończ animację upgrade po jednym cyklu
            isUpgrading = false;
            currentAnim = AnimationType::Idle;
            loadAnimation(AnimationType::Idle);
            return;
        }
    } else {
        // idle zapętlaj
        if (currentFrame >= frameCount) {
            currentFrame = 0;
        }
    }

    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
}



void TowerCatapult::loadAnimation(AnimationType type) {
    std::string path;
    if (type == AnimationType::Idle) {
        path = "assets/CatapultTower/Idle" + std::to_string(level) + ".png";
    } else {
        path = "assets/CatapultTower/Upgrade" + std::to_string(level) + ".png";
    }

    if (!idleTexture.loadFromFile(path)) {
        std::cerr << "[BŁĄD] Nie można załadować tekstury: " << path << "\n";
        return;
    }

    sprite.setTexture(idleTexture);

    // Ustaw liczbę klatek w zależności od poziomu i typu
    if (type == AnimationType::Idle) {
        if (level <= 2)
            frameCount = 1;
        else if (level <= 4)
            frameCount = 4;
    } else {
        frameCount = 4;
    }

    currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

}

void TowerCatapult::upgrade() {
    if (isUpgrading) {
        std::cout << "[INFO] Wieża już się ulepsza\n";
        return;
    }

    if (level < maxLevel) {
        ++level;
        currentAnim = AnimationType::Upgrade;
        isUpgrading = true;
        upgradeTime = 0.f;
        loadAnimation(AnimationType::Upgrade);

        damage = static_cast<int>(damage * 1.2f);
        range *= 1.05f;
        attackCooldown *= 0.95f;
    } else {
        std::cout << "[INFO] Maksymalny poziom osiągnięty\n";
    }
}


bool TowerCatapult::contains(const sf::Vector2f& point) const {
    return sprite.getGlobalBounds().contains(point);
}

