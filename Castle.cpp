#include "Castle.h"
#include <stdexcept>

Castle::Castle(const sf::Vector2f& position)
    : maxHP(500), hp(maxHP), level(1), maxLevel(5) {

    if (!texture.loadFromFile("assets/castle.png")) {
        throw std::runtime_error("Nie można załadować assets/castle.png");
    }

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f); // jeśli chcesz pomniejszyć

    hpBarBg.setSize(sf::Vector2f(60, 8));
    hpBarBg.setFillColor(sf::Color::Red);

    hpBar.setSize(sf::Vector2f(60, 8));
    hpBar.setFillColor(sf::Color::Green);

    hpBarBg.setPosition(position.x - 60, position.y - 140);
    hpBar.setPosition(hpBarBg.getPosition());
}

void Castle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hpBarBg);
    window.draw(hpBar);
}

void Castle::update() {
    updateHpBar();
}

void Castle::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

bool Castle::isDestroyed() const {
    return hp <= 0;
}

void Castle::upgrade() {
    if (level < maxLevel)
        level++;
}

int Castle::getLevel() const {
    return level;
}

int Castle::getMaxLevel() const {
    return maxLevel;
}

bool Castle::canUpgradeTowers() const {
    return level >= 2;
}

bool Castle::canUpgradeGenerators() const {
    return level >= 3;
}

int Castle::getHP() const {
    return hp;
}

int Castle::getMaxHP() const {
    return maxHP;
}

void Castle::updateHpBar() {
    float ratio = static_cast<float>(hp) / maxHP;
    hpBar.setSize(sf::Vector2f(60 * ratio, 8));
}
