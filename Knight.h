#ifndef KNIGHT_H
#define KNIGHT_H

#include "HeroBase.h"
#include <array>

enum class KnightState {
    Idle,
    Walk,
    Attack,
    Dash,
    Dead,
    Win
};

class Knight : public HeroBase {
public:
    Knight(const sf::Vector2f& spawnPoint, const sf::Texture& texture);

    void update(float deltaTime, const sf::RenderWindow& window,
                std::vector<std::unique_ptr<Enemy>>& enemies) override;

    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;
    bool isDead() const override;
    void respawn() override;

protected:
    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                      const sf::RenderWindow& window) override;

    void updateAnimation(float deltaTime) override;

    // Animacja i stany
    KnightState state = KnightState::Idle;

    sf::Texture walk1Tex, walk2Tex;
    std::array<sf::Texture, 4> attackTex;
    sf::Texture deathTex;
    sf::Texture winTex;
    sf::Texture idleTex;


    int frameCount = 6;
    int currentFrame = 0;
    float animationTimer = 0.f;
    float frameTime = 0.1f;
    sf::Vector2i frameSize = {100, 64};

    void switchState(KnightState newState);
    void updateTexture();
    void handleMovement(float deltaTime) override;
    void queueAttack() override;
    void queueExtraAttack() override { /* może pusta implementacja */ }

};

#endif
