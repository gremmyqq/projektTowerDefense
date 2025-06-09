#include "Field.h"
#include <cmath>
#include <iostream>
sf::Texture Field::fieldTexture;


Field::Field(const sf::Vector2f& position)
    : type(FieldType::Empty),
    attackCooldown(1.0f), timeSinceLastAttack(0.0f), range(100.f), damage(15),
    resourceTimer(0.f), resourceInterval(5.f), resourceCount(nullptr) {

    shape.setSize({50.f, 50.f});
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Transparent);

    if (fieldTexture.getSize().x == 0) {
        if (!fieldTexture.loadFromFile("assets/field_transparent.png")) {
            std::cerr << "Nie można załadować assets/field.png\n";
        }
    }

    fieldSprite.setTexture(fieldTexture);
    fieldSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    fieldSprite.setOrigin(32.f, 32.f);
    fieldSprite.setPosition(position);



}

void Field::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (type == FieldType::Tower) {
        timeSinceLastAttack += deltaTime;
        attackEnemies(enemies);
    } else if (type == FieldType::Generator) {
        resourceTimer += deltaTime;
        generateResources();
    }
}

void Field::draw(sf::RenderWindow& window) {
    if (fieldTexture.getSize().x > 0)
        window.draw(fieldSprite);
    else
        window.draw(shape); // awaryjnie
}

bool Field::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

void Field::handleClick(const sf::Vector2f& mousePos) {
    if (!contains(mousePos) || type != FieldType::Empty)
        return;

    // Tymczasowe menu: zawsze buduj wieżę
    std::cout << "Kliknięto pole. Buduję wieżę." << std::endl;
    buildTower();
    // buildGenerator(); // testowo
}

void Field::buildTower() {
    type = FieldType::Tower;
    shape.setFillColor(sf::Color::Blue);
}

void Field::buildGenerator() {
    type = FieldType::Generator;
    shape.setFillColor(sf::Color::Green);
    // resourceCount = ... // przypisz wskaźnik w konstruktorze
}

void Field::attackEnemies(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;

        sf::Vector2f toEnemy = enemy->getPosition() - shape.getPosition();
        float distance = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

        if (distance <= range && timeSinceLastAttack >= attackCooldown) {
            enemy->takeDamage(damage);
            timeSinceLastAttack = 0.f;
            break;
        }
    }
}

void Field::generateResources() {
    if (resourceTimer >= resourceInterval && resourceCount) {
        *resourceCount += 1;
        resourceTimer = 0.f;
    }
}
