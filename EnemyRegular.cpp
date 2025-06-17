//
// Created by CP on 30.05.2025.
//

#include "EnemyRegular.h"

EnemyRegular::EnemyRegular(const std::vector<sf::Vector2f>& path)
    : Enemy(path) {
    maxHealth = 250;
    health = maxHealth;
    speed = 100;

    texture.loadFromFile("assets/enemy_regular.png");
    sprite.setTexture(texture);
    frameWidth = 48;
    frameHeight = 48;
    totalFrames = 6;
    animationInterval = 0.2f;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setScale(2.f, 2.f);
    if (!path.empty()) setPosition(path[0]);
}



void EnemyRegular::update(float deltaTime) {
    if (!reachedEnd() && !isDead()) {
        moveTowardsTarget(deltaTime);
    }
    updateAnimation(deltaTime);
}


void EnemyRegular::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(sprite);
}


