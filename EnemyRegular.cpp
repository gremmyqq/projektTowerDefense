//
// Created by CP on 30.05.2025.
//

#include "EnemyRegular.h"
#include <iostream>

EnemyRegular::EnemyRegular(const std::vector<sf::Vector2f>& path)
    : Enemy(path) {
    maxHealth = 100;
    health = maxHealth;
    speed = 100;
    if (!texture.loadFromFile("assets/enemy_regular.png")) {
        std::cout << "Nie można załadować enemy_regular.png\n";
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth, frameHeight);
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(position);


    if (!path.empty()) {
        setPosition(path[0]);  // <- jawne ustawienie pozycji
    }
}


void EnemyRegular::update(float deltaTime) {
    if (!reachedEnd() && !isDead()) {
        moveTowardsTarget(deltaTime);
    }

    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        currentFrame = (currentFrame + 1) % totalFrames;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        animationTimer = 0.f;
    }
    sprite.setPosition(position);

}

void EnemyRegular::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(sprite);
}
