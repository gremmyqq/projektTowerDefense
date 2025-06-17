#ifndef MAGE_H
#define MAGE_H

#include "HeroBase.h"
#include "Spell.h"
#include <vector>
#include <memory>

enum class MageState {
    Idle,
    Walk,
    Dash,
    Attack,
    Dead,
    Win
};

class Mage : public HeroBase {
public:
    Mage(const sf::Vector2f& spawnPoint, const sf::Texture& dummy);
    void update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;
    bool isDead() const override;
    void respawn() override;
    void queueAttack() override;
    void queueExtraAttack() override;

private:
    MageState state;
    sf::Texture idleTex, walkTex, runTex, attackTex, deathTex, winTex;
    sf::Texture fireballTex;  // jedna tekstura fireballa

    std::vector<std::unique_ptr<Spell>> activeSpells;

    int damage;
    float range;
    float speed;
    float attackCooldown;
    float attackTimer;
    bool attackQueued = false;

    float dashSpeed = 400.f;
    float dashDuration = 0.2f;
    float dashTime = 0.f;
    bool dashing = false;

    int frameCount = 6;
    int currentFrame = 0;
    sf::Vector2i frameSize;
    float animationTimer = 0.f;
    float frameTime = 0.1f;
    bool fireballQueued = false;

    sf::Vector2f spawnPosition;

    void switchState(MageState newState);
    void updateTexture();
    void handleMovement(float deltaTime, const sf::RenderWindow& window);
    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window);
    void updateAnimation(float deltaTime);
};

#endif
