#include "Arrow.h"
#include <cmath>

Arrow::Arrow(const sf::Vector2f& startPos, const sf::Vector2f& targetPos, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(startPos);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

    sf::Vector2f dir = targetPos - startPos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    velocity = dir / len * speed;

    float angle = std::atan2(dir.y, dir.x) * 180 / 3.14159f;
    sprite.setRotation(angle);
}

void Arrow::update(float deltaTime) {
    sprite.move(velocity * deltaTime);
}

void Arrow::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Arrow::checkCollision(Enemy& enemy) {
    sf::FloatRect enemyBounds(enemy.getPosition().x - 20, enemy.getPosition().y - 20, 40, 40);
    return sprite.getGlobalBounds().intersects(enemyBounds);

}

bool Arrow::isOffScreen(const sf::RenderWindow& window) const {
    sf::Vector2f pos = sprite.getPosition();
    return pos.x < 0 || pos.y < 0 || pos.x > window.getSize().x || pos.y > window.getSize().y;
}

sf::Vector2f Arrow::getPosition() const {
    return sprite.getPosition();
}
void Arrow::markForRemoval() {
    markedForRemoval = true;
}

bool Arrow::isMarked() const {
    return markedForRemoval;
}

bool Arrow::isFinished() const {
    return reachedTarget;
}
