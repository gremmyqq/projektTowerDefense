//
// Created by CP on 29.05.2025.
//

#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

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
    int totalFrames = 3;
    int frameWidth = 48;
    int frameHeight = 48;
    int direction = 0;

    enum class EnemyState {
        Walking,
        Attacking,
        Dying
    };

    EnemyState state = EnemyState::Walking;

    void moveTowardsTarget(float deltaTime);
    void updateAnimation(float deltaTime);
    void updateSpriteDirection(const sf::Vector2f& dir);
};

#endif //ENEMY_H
