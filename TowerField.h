#ifndef TOWERFIELD_H
#define TOWERFIELD_H

#include "Field.h"
#include <SFML/Graphics.hpp>

class TowerField : public Field {
public:
    explicit TowerField(const sf::Vector2f& pos);

    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const override;
    void handleClick(BuildType, GameEngine&) override;
    virtual void upgrade(GameEngine* engine) = 0;


protected:
    virtual void attack(std::vector<std::unique_ptr<Enemy>>& enemies);
    sf::RectangleShape shape;
    float attackCooldown = 1.0f;
    float timeSinceLastAttack = 0.0f;
    float range = 100.f;
    int damage = 15;

};

#endif
