//
// Created by CP on 29.05.2025.
//

#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path);
    virtual ~Enemy() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual void takeDamage(int damage);
    virtual bool isDead() const;
    virtual bool reachedEnd() const;

    sf::Vector2f getPosition() const;

protected:
    std::vector<sf::Vector2f> path;
    size_t currentTargetIndex;

    sf::Vector2f position;
    float speed;
    int health;
    int maxHealth;

    void moveTowardsTarget(float deltaTime);
};

#endif //ENEMY_H
