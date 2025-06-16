#include "EnemyArcher.h"
#include <cmath>
#include <iostream>
#include "HeroBase.h"

EnemyArcher::EnemyArcher(const std::vector<sf::Vector2f>& path)
    : Enemy(path)
{
    maxHealth = 120;
    health = maxHealth;
    speed = 90;

    if (!texture.loadFromFile("assets/enemy_archer.png")) {
        std::cerr << "Nie można załadować enemy_archer.png\n";
    }
    if (!arrowTexture.loadFromFile("assets/ArcherTower/arrow.png")) {
        std::cerr << "Nie można załadować arrow.png\n";
    }

    sprite.setTexture(texture);
    frameWidth = 48;
    frameHeight = 48;
    totalFrames = 6;
    animationInterval = 0.2f;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setScale(2.f, 2.f);
    if (!path.empty()) setPosition(path[0]);
}

void EnemyArcher::update(float deltaTime) {
    if (!reachedEnd() && !isDead()) {
        moveTowardsTarget(deltaTime);
    }
    updateAnimation(deltaTime);

    for (auto& arrow : arrows)
        arrow.update(deltaTime);

    // usuwanie
    arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
                                [](const Arrow& a) { return a.isFinished(); }), arrows.end());
}

void EnemyArcher::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (auto& arrow : arrows)
        arrow.draw(window);
}

void EnemyArcher::updateAgainstHero(float deltaTime, HeroBase& hero) {
    if (state == EnemyState::Dying) return;

    shootTimer += deltaTime;

    sf::Vector2f toHero = hero.getPosition() - position;
    float distance = std::sqrt(toHero.x * toHero.x + toHero.y * toHero.y);

    if (distance <= attackRange) {
        //std::cout<<"achtung"; //działa
        updateSpriteDirection(toHero);

        if (shootTimer >= shootCooldown) {
            state = EnemyState::Attacking;
            currentFrame = 0;
            animationTimer = 0.f;

            shootTimer = 0.f;
            // NIE tworzymy strzały tutaj!
        }


    }

    // sprawdzanie trafienia
    for (auto& arrow : arrows) {
        float dist = std::hypot(
            hero.getPosition().x - arrow.getPosition().x,
            hero.getPosition().y - arrow.getPosition().y
            );

        if (dist < 20.f) {
            hero.takeDamage(attackDamage);
            arrow = arrows.back(); // optymalnie: usuń przez swap
            arrows.pop_back();
            break;
        }
    }
    lastHeroPosition = hero.getPosition(); // dodaj pole sf::Vector2f lastHeroPosition;

}
void EnemyArcher::shootArrow() {
    arrows.emplace_back(position, lastHeroPosition, arrowTexture);
}
void EnemyArcher::onAttackAnimationFinished() {
    shootArrow();
}


