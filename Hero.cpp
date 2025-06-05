#include "Hero.h"
#include <cmath>

// Konstruktor ustawia wszystkie statystyki, sprite i pasek HP
Hero::Hero(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : speed(150.f), maxHp(100), hp(maxHp), damage(20),
    attackCooldown(0.5f), attackTimer(0.f), level(1), range(50.f),
    spawnPosition(spawnPoint) {

    sprite.setTexture(texture);
    sprite.setPosition(spawnPoint);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

    hpBarBg.setSize(sf::Vector2f(40, 5));
    hpBarBg.setFillColor(sf::Color::Red);
    hpBar.setSize(sf::Vector2f(40, 5));
    hpBar.setFillColor(sf::Color::Green);
}

// Główna aktualizacja: ruch, atak i pasek zdrowia
void Hero::update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (isDead()) return;

    handleMovement(deltaTime);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
}

// Sterowanie ruchem (WSAD)
void Hero::handleMovement(float deltaTime) {
    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    // Normalizacja kierunku i przesunięcie sprite’a
    if (moveDir.x != 0.f || moveDir.y != 0.f) {
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= len;
        sprite.move(moveDir * speed * deltaTime);
    }
}

// Atak myszką — jeśli wróg w zasięgu, zadaj obrażenia
void Hero::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && attackTimer >= attackCooldown) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;
            float dist = std::hypot(enemy->getPosition().x - sprite.getPosition().x,
                                    enemy->getPosition().y - sprite.getPosition().y);
            if (dist <= range) {
                enemy->takeDamage(damage);
                attackTimer = 0.f;
                break; // atakujemy tylko jednego wroga
            }
        }
    }
}

// Rysowanie bohatera i paska HP
void Hero::draw(sf::RenderWindow& window) {
    if (isDead()) return;
    window.draw(sprite);

    // Ustaw pozycję paska HP nad bohaterem
    hpBarBg.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y - 40);
    hpBar.setPosition(hpBarBg.getPosition());

    window.draw(hpBarBg);
    window.draw(hpBar);
}

// Odejmowanie HP
void Hero::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

// Czy nie zyje
bool Hero::isDead() const {
    return hp <= 0;
}

// Reset lvla po smierci
void Hero::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
}

// Przeskalowanie paska HP
void Hero::updateHpBar() {
    float ratio = static_cast<float>(hp) / maxHp;
    hpBar.setSize({40.f * ratio, 5.f});
}
