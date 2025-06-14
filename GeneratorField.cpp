#include "GeneratorField.h"

GeneratorField::GeneratorField(const sf::Vector2f& pos) {
    shape.setSize({50.f, 50.f});
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Green);
}

void GeneratorField::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>&) {
    resourceTimer += deltaTime;
    generateResources();
}

void GeneratorField::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool GeneratorField::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

void GeneratorField::handleClick(BuildType selectedType, GameEngine& engine) {
    // Już generator — nic nie rób
}


void GeneratorField::generateResources() {
    if (resourceTimer >= resourceInterval && resourceCount) {
        (*resourceCount) += 1;
        resourceTimer = 0.f;
    }
}
