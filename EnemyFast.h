#ifndef ENEMYFAST_H
#define ENEMYFAST_H

#include "Enemy.h"

class EnemyFast : public Enemy {
public:
    EnemyFast(const std::vector<sf::Vector2f>& path);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::CircleShape shape;
};

#endif // ENEMYFAST_H
