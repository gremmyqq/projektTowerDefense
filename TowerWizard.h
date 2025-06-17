#ifndef TOWERWIZARD_H
#define TOWERWIZARD_H

#include "TowerField.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Arrow.h"



class TowerWizard : public TowerField {
public:
    explicit TowerWizard(const sf::Vector2f& pos);

    void attack(std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
    void upgrade(GameEngine* engine);
    bool contains(const sf::Vector2f& point) const override;

private:
    sf::Texture idleTexture;
    sf::Sprite sprite;

    std::vector<Arrow> arrows;
    sf::Texture arrowTexture;

    float animTimer = 0.f;
    float animInterval = 0.2f;
    int currentFrame = 0;
    int frameWidth = 70;
    int frameHeight = 130;
    int frameCount = 1;

    int level = 1;
    int maxLevel = 4;

    bool isUpgrading = false;
    float upgradeTime = 0.f;
    float upgradeDuration = 1.0f;

    enum class AnimationType { Idle, Upgrade };
    AnimationType currentAnim = AnimationType::Idle;

    void updateAnimation(float deltaTime);
    void loadAnimation(AnimationType type);

    float attackCooldown = .1f;
    float timeSinceLastAttack = 0.0f;
    float range = 200.f;
    int damage = 5;
};

#endif // TOWERWIZARD_H
