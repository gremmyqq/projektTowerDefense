#include "Spell.h"
#include <cmath>

Spell::Spell(const sf::Vector2f& startPos, const sf::Vector2f& targetPos, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(startPos);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setScale(0.1f, 0.1f);  // Przykładowa skala — możesz dobrać inną


    sf::Vector2f dir = targetPos - startPos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    velocity = (len > 0.f) ? dir / len * speed : sf::Vector2f(0.f, 0.f);

    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;
    sprite.setRotation(angle);
}

void Spell::update(float deltaTime) {
    sprite.move(velocity * deltaTime);
}

void Spell::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Spell::checkCollision(Enemy& enemy) {
    sf::FloatRect enemyBounds(enemy.getPosition().x - 20, enemy.getPosition().y - 20, 40, 40);
    if (sprite.getGlobalBounds().intersects(enemyBounds)) {
        markedForRemoval = true;
        return true;
    }
    return false;
}

bool Spell::isMarked() const {
    return markedForRemoval;
}
