//
// Created by CP on 30.05.2025.
//

#ifndef ENEMYREGULAR_H
#define ENEMYREGULAR_H

#include "Enemy.h"

class EnemyRegular : public Enemy {
public:
    EnemyRegular(const std::vector<sf::Vector2f>& path);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;

private:
    sf::CircleShape shape;
};

#endif //ENEMYREGULAR_H
