#include "HeroBase.h"
#include <cmath>

HeroBase::HeroBase(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : spawnPosition(spawnPoint) {

    sprite.setTexture(texture);
    sprite.setPosition(spawnPoint);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

    hpBarBg.setSize(sf::Vector2f(40.f, 5.f));
    hpBarBg.setFillColor(sf::Color::Red);

    hpBar.setSize(sf::Vector2f(40.f, 5.f));
    hpBar.setFillColor(sf::Color::Green);
}

void HeroBase::updateHpBar() {
    float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
    hpBar.setSize({40.f * ratio, 5.f});
}

void HeroBase::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

bool HeroBase::isDead() const {
    return hp <= 0;
}

void HeroBase::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
}

sf::Vector2f HeroBase::getPosition() const {
    return sprite.getPosition();
}

void HeroBase::handleMovement(float deltaTime) {
    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    if (moveDir.x != 0.f || moveDir.y != 0.f) {
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= len;
        sprite.move(moveDir * speed * deltaTime);
    }
}

void HeroBase::queueAttack() {
    attackQueued = true;
}

