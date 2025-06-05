#ifndef CASTLE_H
#define CASTLE_H

#include <SFML/Graphics.hpp>

class Castle {
public:
    Castle(const sf::Vector2f& position);

    void draw(sf::RenderWindow& window);
    void update();

    void takeDamage(int dmg);
    bool isDestroyed() const;

    void upgrade();  // podnosi poziom zamku
    int getLevel() const;
    int getMaxLevel() const;

    bool canUpgradeTowers() const;
    bool canUpgradeGenerators() const;

    int getHP() const;
    int getMaxHP() const;

private:
    sf::Sprite sprite;
    sf::RectangleShape hpBarBg, hpBar;

    int maxHP;
    int hp;

    int level;
    int maxLevel;

    void updateHpBar();
};

#endif // CASTLE_H
