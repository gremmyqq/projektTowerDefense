//
// Created by CP on 29.05.2025.
//

#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int maxHealth);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    void takeDamage(int damage);
    bool isDead() const;
    bool reachedEnd() const;

    sf::Vector2f getPosition() const;

private:
    std::vector<sf::Vector2f> path;
    size_t currentTargetIndex;

    sf::Vector2f position;
    float speed;
    int health;
    int maxHealth;

    sf::CircleShape shape;

    void moveTowardsTarget(float deltaTime);
};

#endif //ENEMY_H
