//
// Created by CP on 30.05.2025.
//

#ifndef ENEMYREGULAR_H
#define ENEMYREGULAR_H

#include "Enemy.h"

class EnemyRegular : public Enemy {
public:
    EnemyRegular(const std::vector<sf::Vector2f>& path);

    sf::Texture texture;
    sf::Sprite sprite;

    float animationTimer = 0.f;
    float animationInterval = 0.2f; // zmiana co 0.2 sekundy
    int currentFrame = 0;
    int totalFrames = 5;
    int frameWidth = 48;
    int frameHeight = 48;


    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::CircleShape shape;
};

#endif //ENEMYREGULAR_H
