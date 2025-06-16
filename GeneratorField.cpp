#include "GeneratorField.h"
#include <iostream>

GeneratorField::GeneratorField(const sf::Vector2f& pos, GeneratorType type)
    : type(type)
{
    std::string path;

    switch (type) {
    case GeneratorType::Wood:
        path = "assets/Tent/1.png";
        break;
    case GeneratorType::Iron:
        path = "assets/Tent/2.png";
        break;
    case GeneratorType::Food:
        path = "assets/Tent/3.png";
        break;
    }

    if (!texture.loadFromFile(path)) {
        std::cerr << "[BŁĄD] Nie można załadować tekstury: " << path << "\n";
    }

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(pos);
    sprite.setScale(1.0f, 1.0f); // opcjonalnie dostosuj
}

void GeneratorField::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    resourceTimer += deltaTime;
    generateResources();
}

void GeneratorField::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool GeneratorField::contains(const sf::Vector2f& point) const {
    return sprite.getGlobalBounds().contains(point);
}

void GeneratorField::handleClick(BuildType selectedType, GameEngine& engine) {
    // Generator nie podlega przebudowie
}

void GeneratorField::generateResources() {
    if (resourceTimer >= resourceInterval && resourceCount) {
        (*resourceCount) += 10;
        resourceTimer = 0.f;
    }
}

void GeneratorField::setResourcePointer(int* resourcePtr) {
    resourceCount = resourcePtr;
}
