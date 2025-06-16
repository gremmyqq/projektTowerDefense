#ifndef GENERATORFIELD_H
#define GENERATORFIELD_H

#include "Field.h"
enum class GeneratorType {
    Wood,
    Iron,
    Food
};

class GeneratorField : public Field {
public:
    GeneratorField(const sf::Vector2f& pos, GeneratorType type);

    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>&) override;
    void draw(sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const override;
    void handleClick(BuildType selectedType, GameEngine& engine) override;
    void setResourcePointer(int* resourcePtr);

private:
    GeneratorType type;
    sf::Texture texture;
    sf::Sprite sprite;

    float resourceTimer = 0.f;
    float resourceInterval = 3.f;
    int* resourceCount = nullptr;


    void generateResources();
};

#endif
