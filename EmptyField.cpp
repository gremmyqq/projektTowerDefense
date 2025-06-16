#include "EmptyField.h"
#include "TowerArcher.h"
#include "TowerCatapult.h"
#include "TowerField.h"
#include "GeneratorField.h"
#include "TowerWizard.h"
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
    fieldSprite.setOrigin(fieldTexture.getSize().x / 2.f, fieldTexture.getSize().y / 2.f); // dostosuj do grafiki
    fieldSprite.setPosition(pos);
    fieldSprite.setScale(1.25f, 1.25f);
}

void EmptyField::draw(sf::RenderWindow& window) {
    window.draw(fieldSprite);
}

bool EmptyField::contains(const sf::Vector2f& point) const {
    std::cout << "[DEBUG] contains(): pole x=" << fieldSprite.getPosition().x
              << ", y=" << fieldSprite.getPosition().y << "\n";
    std::cout << "[DEBUG] point: " << point.x << ", " << point.y << "\n";
    return fieldSprite.getGlobalBounds().contains(point);
}


void EmptyField::handleClick(BuildType selectedType, GameEngine& engine) {
    std::cout << "[DEBUG] EmptyField::handleClick wywołane\n";

    if (selectedType == BuildType::Generator) {
        engine.replaceField(this, std::make_unique<GeneratorField>(fieldSprite.getPosition()));
    } else if (selectedType == BuildType::TowerArcher) {
        engine.replaceField(this, std::make_unique<TowerArcher>(fieldSprite.getPosition()));
    } else if (selectedType == BuildType::TowerWizard) {
        engine.replaceField(this, std::make_unique<TowerWizard>(fieldSprite.getPosition()));
    } else if (selectedType == BuildType::TowerCatapult) {
        engine.replaceField(this, std::make_unique<TowerCatapult>(fieldSprite.getPosition()));
    }

}
