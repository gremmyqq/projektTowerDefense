#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    explicit Enemy(const std::vector<sf::Vector2f>& path);
    virtual ~Enemy() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void takeDamage(int damage);
    virtual bool isDead() const;
    virtual bool reachedEnd() const;

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);

protected:
    std::vector<sf::Vector2f> path;
    sf::Vector2f position;
    size_t currentTargetIndex = 0;
    float speed = 100.f;

    int health = 100;
    int maxHealth = 100;
    void moveTowardsTarget(float deltaTime);
};

#endif // ENEMY_H
