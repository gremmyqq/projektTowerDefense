#ifndef TOWERARCHER_H
#define TOWERARCHER_H

#include "TowerField.h"
#include <SFML/Graphics.hpp>

class TowerArcher : public TowerField {
public:
    explicit TowerArcher(const sf::Vector2f& pos);

    void attack(std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
    void upgrade();
    bool contains(const sf::Vector2f& point) const override;


private:
    sf::Texture texture;
    sf::Sprite sprite;

    float animTimer = 0.f;
    float animInterval = 0.2f;
    int currentFrame = 4; // idle start
    const int frameWidth = 70;
    const int frameHeight = 130;

    bool isUpgrading = false;
    float upgradeTime = 0.f;
    float upgradeDuration = 1.0f;

    int level = 1;
    const int maxLevel = 8;

    enum class AnimationType { Idle, Upgrade };
    AnimationType currentAnim = AnimationType::Idle;

    void updateAnimation(float deltaTime);

};

#endif // TOWERARCHER_H
