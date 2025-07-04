#ifndef ARROW_H
#define ARROW_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Arrow {
public:
    Arrow(const sf::Vector2f& startPos, const sf::Vector2f& targetPos, const sf::Texture& texture, float speed = 1000.f);


    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkCollision(Enemy& enemy);
    bool isOffScreen(const sf::RenderWindow& window) const;
    void markForRemoval();
    bool isMarked() const;
    bool isFinished() const;
    bool reachedTarget = false;


    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = 1000.f;
    bool markedForRemoval = false;
    sf::Vector2f target;
};

#endif
