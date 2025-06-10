#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Enemy.h"

class Hero {
public:
    Hero(const sf::Vector2f& spawnPoint, const sf::Texture& texture);
    void update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);
    void takeDamage(int dmg);
    bool isDead() const;
    void respawn();

private:
    // Sprite i animacja
    sf::Sprite sprite;
    sf::Vector2i frameSize;
    int currentFrame;
    int frameCount;
    int animationRow;
    float frameTime;
    float animationTimer;

    enum class HeroAnimationState { Idle, Run, Attack };
    HeroAnimationState animationState;

    // Pasek zdrowia
    sf::RectangleShape hpBarBg, hpBar;

    // Statystyki
    float speed;
    int maxHp;
    int hp;
    int damage;
    float attackCooldown;
    float attackTimer;
    int level;
    float range;

    sf::Vector2f spawnPosition;

    // Sterowanie
    void handleMovement(float deltaTime);
    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window);
    void updateHpBar();
    void updateAnimation(float deltaTime);
};

#endif // HERO_H
