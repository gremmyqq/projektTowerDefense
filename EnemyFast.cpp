#include "EnemyFast.h"

EnemyFast::EnemyFast(const std::vector<sf::Vector2f>& path)
    : Enemy(path) {
    maxHealth = 120;
    health = maxHealth;
    speed = 150;

    texture.loadFromFile("assets/enemy_fast.png");
    sprite.setTexture(texture);
    frameWidth = 48;
    frameHeight = 48;
    totalFrames = 6;
    animationInterval = 0.15f;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setScale(2.f, 2.f);
    if (!path.empty()) setPosition(path[0]);
}



void EnemyFast::update(float deltaTime) {
    if (!reachedEnd() && !isDead()) {
        moveTowardsTarget(deltaTime);
    }
    updateAnimation(deltaTime);
}


void EnemyFast::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(sprite);
}


