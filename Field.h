#ifndef FIELD_H
#define FIELD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"
#include "BuildType.h"
class GameEngine;

class Field {
public:
    virtual ~Field() = default;
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool contains(const sf::Vector2f& point) const = 0;
    virtual void handleClick(BuildType selectedType, GameEngine& engine) = 0;
};

#endif
