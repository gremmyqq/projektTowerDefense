#ifndef MAGE_H
#define MAGE_H

#include "HeroBase.h"
#include <array>

enum class MageState {
    Idle,
    Run,
    Attack,
    ExtraAttack,
    Dead
};

class Mage : public HeroBase {
public:
    Mage(const sf::Vector2f& spawnPoint, const sf::Texture& texture);

    void update(float deltaTime, const sf::RenderWindow& window,
                std::vector<std::unique_ptr<Enemy>>& enemies) override;

    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;
    bool isDead() const override;
    void respawn() override;

protected:
    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                      const sf::RenderWindow& window) override;

    void handleMovement(float deltaTime) override;
    void updateAnimation(float deltaTime) override;
    void queueAttack() override;

    // Animacja i stany
    MageState state = MageState::Idle;

    sf::Texture idleTex, runTex, attackTex, extraAttackTex, deathTex;

    int frameCount = 6;
    int currentFrame = 0;
    float animationTimer = 0.f;
    float frameTime = 0.1f;
    sf::Vector2i frameSize = {128, 128};

    void switchState(MageState newState);
    void updateTexture();
    bool normalAttackQueued = false;
    bool extraAttackQueued = false;

};

#endif // MAGE_H
