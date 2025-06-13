#include "Archer.h"
#include "HeroBase.h"
#include <cmath>

Archer::Archer(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : HeroBase(spawnPoint, texture) {
    speed = 160.f;
    maxHp = 80;
    hp = maxHp;
    damage = 15;
    range = 120.f;
    attackCooldown = 0.8f;
}

void Archer::update(float deltaTime, const sf::RenderWindow& window,
                    std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (isDead()) return;

    handleMovement(deltaTime);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);
}

void Archer::draw(sf::RenderWindow& window) {
    if (isDead()) return;

    window.draw(sprite);
    hpBarBg.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y - 40);
    hpBar.setPosition(hpBarBg.getPosition());

    window.draw(hpBarBg);
    window.draw(hpBar);
}

void Archer::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

bool Archer::isDead() const {
    return hp <= 0;
}

void Archer::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
}

void Archer::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                          const sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && attackTimer >= attackCooldown) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;

            float dist = std::hypot(enemy->getPosition().x - sprite.getPosition().x,
                                    enemy->getPosition().y - sprite.getPosition().y);

            if (dist <= range) {
                enemy->takeDamage(damage);
                attackTimer = 0.f;
                break;
            }
        }
    }
}

void Archer::updateAnimation(float deltaTime) {
    // opcjonalna animacja
}
