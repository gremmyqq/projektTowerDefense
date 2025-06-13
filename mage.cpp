#include "Mage.h"
#include <cmath>

Mage::Mage(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : HeroBase(spawnPoint, texture) {
    speed = 140.f;
    maxHp = 100;
    hp = maxHp;
    damage = 25;
    range = 200.f;
    attackCooldown = 1.2f;
}

void Mage::update(float deltaTime,
                  const sf::RenderWindow& window,
                  std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (isDead()) return;

    handleMovement(deltaTime);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);
}

void Mage::draw(sf::RenderWindow& window) {
    if (isDead()) return;

    window.draw(sprite);
    hpBarBg.setPosition(sprite.getPosition().x - 20.f, sprite.getPosition().y - 40.f);
    hpBar.setPosition(hpBarBg.getPosition());

    window.draw(hpBarBg);
    window.draw(hpBar);
}

void Mage::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

bool Mage::isDead() const {
    return hp <= 0;
}

void Mage::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
}

void Mage::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
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

void Mage::updateAnimation(float deltaTime) {
    // Brak animacji na razie
}
