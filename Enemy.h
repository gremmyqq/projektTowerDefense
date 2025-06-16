//
// Created by CP on 29.05.2025.
//

#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class HeroBase;  // forward declaration

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path);
    virtual ~Enemy() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void takeDamage(int damage);
    virtual bool isDead() const;
    virtual bool reachedEnd() const;

    sf::Vector2f getPosition() const;

    void setPosition(sf::Vector2f newPos);
    // --- Nowe metody animacyjne ---
    virtual void startAttack();
    virtual void die();
    bool markedForDeletion = false;

    virtual void updateAgainstHero(float deltaTime, HeroBase& hero);
    virtual void onAttackAnimationFinished() {}  // domyślnie nic nie robi



    // ... (dotychczasowy kod)
protected:
    std::vector<sf::Vector2f> path;
    size_t currentTargetIndex;

    sf::Vector2f position;
    float speed;
    int health;
    int maxHealth;

    sf::Texture texture;
    sf::Sprite sprite;

    float animationTimer = 0.f;
    float animationInterval = 0.2f;
    int currentFrame = 0;
    int totalFrames = 6;
    int frameWidth = 48;
    int frameHeight = 48;
    int direction = 0;

    enum class EnemyState {
        Walking,
        Attacking,
        Dying,
        Special1,
        Special2
    };
    bool canAttack=true;
    EnemyState state = EnemyState::Walking;

    void moveTowardsTarget(float deltaTime);
    void updateAnimation(float deltaTime);
    void updateSpriteDirection(const sf::Vector2f& dir);

    float attackCooldown = 1.0f;
    float attackTimer = 0.0f;
    float attackRange = 50.0f;
    int attackDamage = 10;


};

#endif //ENEMY_H
