#ifndef SAMURAI_H
#define SAMURAI_H

#include "HeroBase.h"

class Samurai : public HeroBase {
public:
    Samurai(const sf::Vector2f& spawnPoint, const sf::Texture& dummy);

    void update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies) override;

    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;
    bool isDead() const override;
    void respawn() override;

    void queueAttack(sf::Keyboard::Key key);
    void queueAttack() override {} // wymagane przez HeroBase
        // wymagane przez HeroBase
    void queueExtraAttack() override {}    // wymagane przez HeroBase
    // Q lub E

private:
    enum class SamuraiState { Idle, Walk, Dash, Attack, Dead, Win };
    enum class AttackVariant { Type1, Type2 };
    void handleAttack(std::vector<std::unique_ptr<Enemy>>&, const sf::RenderWindow&) override;

    void handleMovement(float deltaTime, const sf::RenderWindow& window);

    void updateAnimation(float deltaTime);
    void switchState(SamuraiState newState);
    void updateTexture();
    void performAttack(AttackVariant type, std::vector<std::unique_ptr<Enemy>>& enemies);


    SamuraiState state;
    AttackVariant attackType = AttackVariant::Type1;

    sf::Texture idleTex, walkTex, runTex;
    sf::Texture attack1Tex, attack2Tex;
    sf::Texture deathTex;

    bool attackQueued = false;
    float attackCooldown, attackTimer;

    sf::Vector2f spawnPosition;
};

#endif
