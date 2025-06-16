#ifndef ENEMYARCHER_H
#define ENEMYARCHER_H

#include "Enemy.h"
#include "Arrow.h"
#include <vector>

class EnemyArcher : public Enemy {
public:
    EnemyArcher(const std::vector<sf::Vector2f>& path);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void updateAgainstHero(float deltaTime, HeroBase& hero) override;
    void shootArrow();
    void onAttackAnimationFinished() override;



private:
    std::vector<Arrow> arrows;
    sf::Texture arrowTexture;
    float shootCooldown = 2.0f;
    float shootTimer = 0.f;
    float attackRange = 100.0f;
    sf::Vector2f lastHeroPosition;

};

#endif
