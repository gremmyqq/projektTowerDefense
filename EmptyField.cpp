#include "EmptyField.h"
#include "TowerArcher.h"
#include "TowerField.h"
#include "GeneratorField.h"
#include <iostream>

// definicja statycznej tekstury
sf::Texture EmptyField::fieldTexture;

EmptyField::EmptyField(const sf::Vector2f& pos) {
    if (fieldTexture.getSize().x == 0) {
        if (!fieldTexture.loadFromFile("assets/field.png")) {
            std::cerr << "Nie można załadować assets/field.png\n";
        }
    }

    fieldSprite.setTexture(fieldTexture);
    fieldSprite.setOrigin(32.f, 32.f); // dostosuj do grafiki
    fieldSprite.setPosition(pos);
}

void EmptyField::draw(sf::RenderWindow& window) {
    window.draw(fieldSprite);
}

bool EmptyField::contains(const sf::Vector2f& point) const {
    return fieldSprite.getGlobalBounds().contains(point);
}

void EmptyField::handleClick(BuildType selectedType, GameEngine& engine) {
    if (selectedType == BuildType::Generator) {
        engine.replaceField(this, std::make_unique<GeneratorField>(fieldSprite.getPosition()));
    } else if (selectedType == BuildType::TowerArcher) {
        engine.replaceField(this, std::make_unique<TowerArcher>(fieldSprite.getPosition()));
    }

}
