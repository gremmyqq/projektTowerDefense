#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include "Enemy.h"

class EnemyTank : public Enemy {
public:
    EnemyTank(const std::vector<sf::Vector2f>& path);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void updateAnimationTank(float deltaTime);

private:
    sf::CircleShape shape;
};

#endif // ENEMYTANK_H
