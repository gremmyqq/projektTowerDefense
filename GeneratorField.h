#ifndef GENERATORFIELD_H
#define GENERATORFIELD_H

#include "Field.h"

class GeneratorField : public Field {
public:
    explicit GeneratorField(const sf::Vector2f& pos);

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>&) override;
    void draw(sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const override;
    void handleClick(BuildType selectedType, GameEngine& engine) override;

private:
    sf::RectangleShape shape;
    float resourceTimer = 0.f;
    float resourceInterval = 5.f;
    int* resourceCount = nullptr;

    void generateResources();
};

#endif
