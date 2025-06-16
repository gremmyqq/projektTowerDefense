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



void HeroBase::queueAttack() {
    attackQueued = true;
}

void HeroBase::upgrade() {
    level++;
    maxHp += 20;
    damage += 5;
    range += 10.f;
    hp = maxHp;
}
void HeroBase::updateHpBar() {
    float ratio = static_cast<float>(hp) / maxHp;
    hpBar.setSize({hpBarBg.getSize().x * ratio, hpBarBg.getSize().y});

    sf::Vector2f center = sprite.getPosition();
    hpBarBg.setPosition(center.x - hpBarBg.getSize().x / 2.f, center.y - frameSize.y / 2.f - 12.f);
    hpBar.setPosition(hpBarBg.getPosition());
}

