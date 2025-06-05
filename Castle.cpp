#include "Castle.h"

Castle::Castle(const sf::Vector2f& position)
    : maxHP(500), hp(maxHP), level(1), maxLevel(5) {

    hpBarBg.setSize(sf::Vector2f(60, 8));
    hpBarBg.setFillColor(sf::Color::Red);

    hpBar.setSize(sf::Vector2f(60, 8));
    hpBar.setFillColor(sf::Color::Green);

    hpBarBg.setPosition(position.x - 30, position.y - 70);
    hpBar.setPosition(hpBarBg.getPosition());
}

void Castle::draw(sf::RenderWindow& window) {
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
    return level >= 2;  // np. od poziomu 2 można ulepszać wieże
}

bool Castle::canUpgradeGenerators() const {
    return level >= 3;  // np. od poziomu 3 można ulepszać budynki
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
