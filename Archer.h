#ifndef ARCHER_H
#define ARCHER_H

#include "HeroBase.h"
#include "Arrow.h"  // Strzała jako osobna klasa
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

enum class ArcherState {
    Idle,
    Walk,
    Dash,
    Attack,
    Dead,
    Win
};

class Archer : public HeroBase {
public:
    Archer(const sf::Vector2f& spawnPoint, const sf::Texture& texture);
    void update(float deltaTime, const sf::RenderWindow& window,
                std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;
    bool isDead() const override;
    void respawn() override;
    void queueAttack() override;

private:
    ArcherState state;
    sf::Texture idleTex, runTex, walkTex, deathTex, winTex;
    sf::Texture attackTex, shotTex;

    sf::Texture arrowTexture;
    std::vector<Arrow> arrows;

    float attackCooldown;
    float attackTimer;
    bool attackQueued;


    float range;
    int damage;

    float speed;
    float dashSpeed = 400.f;
    float dashTime = 0.f;
    float dashDuration = 0.2f;
    bool dashing = false;

    sf::Vector2f spawnPosition;

    int frameCount = 6;
    int currentFrame = 0;
    float animationTimer = 0.f;
    float frameTime = 0.1f;
    sf::Vector2i frameSize;

    void handleMovement(float deltaTime, const sf::RenderWindow& window);

    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window);
    void updateAnimation(float deltaTime);
    void updateTexture();
    void switchState(ArcherState newState);
    void queueExtraAttack() override;


};

#endif
