#include "Hero.h"
#include <cmath>

// Konstruktor ustawia wszystkie statystyki, sprite i pasek HP
Hero::Hero(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : frameSize(64, 68), currentFrame(0), frameCount(8), animationRow(0),
    frameTime(0.12f), animationTimer(0.f), animationState(HeroAnimationState::Idle), speed(150.f),
    maxHp(100), hp(maxHp),
    damage(20), attackCooldown(0.5f), attackTimer(0.f), level(1),
    range(50.f), spawnPosition(spawnPoint) {

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, animationRow * frameSize.y, frameSize.x, frameSize.y));
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);
    sprite.setPosition(spawnPoint);

    hpBarBg.setSize(sf::Vector2f(40, 5));
    hpBarBg.setFillColor(sf::Color::Red);
    hpBar.setSize(sf::Vector2f(40, 5));
    hpBar.setFillColor(sf::Color::Green);
}

void Hero::update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (isDead()) return;

    handleMovement(deltaTime);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);

    // Powrót do Idle po animacji ataku
    if (animationState == HeroAnimationState::Attack && currentFrame == frameCount - 1) {
        animationState = HeroAnimationState::Idle;
        animationRow = 0;
        frameCount = 8;
        currentFrame = 0;
        animationTimer = 0.f;
    }
}

void Hero::handleMovement(float deltaTime) {
    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    if (moveDir.x != 0.f || moveDir.y != 0.f) {
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= len;
        sprite.move(moveDir * speed * deltaTime);

        if (animationState != HeroAnimationState::Run) {
            animationState = HeroAnimationState::Run;
            animationRow = 1;
            frameCount = 8;
            currentFrame = 0;
            animationTimer = 0.f;
        }
    } else if (animationState != HeroAnimationState::Attack) {
        animationState = HeroAnimationState::Idle;
        animationRow = 0;
        frameCount = 8;
        currentFrame = 0;
        animationTimer = 0.f;
    }
}

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

                // Zmień na animację ataku
                animationState = HeroAnimationState::Attack;
                animationRow = 4;     // Wiersz z atakiem
                frameCount = 6;       // Liczba klatek ataku
                currentFrame = 0;
                animationTimer = 0.f;

                break;
            }
        }
    }
}

void Hero::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameTime) {
        currentFrame = (currentFrame + 1) % frameCount;
        sprite.setTextureRect(sf::IntRect(
            currentFrame * frameSize.x,
            animationRow * frameSize.y,
            frameSize.x,
            frameSize.y
            ));
        animationTimer = 0.f;
    }
}

void Hero::draw(sf::RenderWindow& window) {
    if (isDead()) return;

    window.draw(sprite);

    hpBarBg.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y - 40);
    hpBar.setPosition(hpBarBg.getPosition());

    window.draw(hpBarBg);
    window.draw(hpBar);
}

void Hero::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}

bool Hero::isDead() const {
    return hp <= 0;
}

void Hero::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
    animationState = HeroAnimationState::Idle;
    animationRow = 0;
    frameCount = 8;
    currentFrame = 0;
    animationTimer = 0.f;
}

void Hero::updateHpBar() {
    float ratio = static_cast<float>(hp) / maxHp;
    hpBar.setSize({40.f * ratio, 5.f});
}
