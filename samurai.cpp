#include "Samurai.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

Samurai::Samurai(const sf::Vector2f& spawnPoint, const sf::Texture&)
    : HeroBase(spawnPoint, sf::Texture()),
    state(SamuraiState::Idle),
    attackCooldown(0.7f),
    attackTimer(0.f),
    spawnPosition(spawnPoint) {

    if (!idleTex.loadFromFile("assets/samurai/Idle.png") ||
        !walkTex.loadFromFile("assets/samurai/Walk.png") ||
        !runTex.loadFromFile("assets/samurai/Run.png") ||
        !attack1Tex.loadFromFile("assets/samurai/Attack_1.png") ||
        !attack2Tex.loadFromFile("assets/samurai/Attack_2.png") ||
        !deathTex.loadFromFile("assets/samurai/Dead.png")) {
        throw std::runtime_error("Nie można załadować tekstur Samuraja");
    }

    speed = 150.f;
    maxHp = 180;
    hp = maxHp;
    damage = 50;
    frameSize = {128, 128};

    switchState(SamuraiState::Idle);
}

void Samurai::update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies)

{
    if (state == SamuraiState::Dead) {
        updateAnimation(deltaTime);
        return;
    }

    handleMovement(deltaTime, window);
    attackTimer += deltaTime;

    if (attackQueued && attackTimer >= attackCooldown) {
        performAttack(attackType, enemies);
        attackQueued = false;
    }

    updateHpBar();
    updateAnimation(deltaTime);

    if (state == SamuraiState::Attack && currentFrame == frameCount - 1) {
        switchState(SamuraiState::Idle);
    }
}

void Samurai::draw(sf::RenderWindow& window)
{
    if (state != SamuraiState::Dead) {
        hpBarBg.setPosition(sprite.getPosition().x - 20.f, sprite.getPosition().y - 60.f);
        hpBar.setPosition(hpBarBg.getPosition());
        window.draw(hpBarBg);
        window.draw(hpBar);
    }

    window.draw(sprite);
}

void Samurai::takeDamage(int dmg)
{
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        switchState(SamuraiState::Dead);
    }
}

bool Samurai::isDead() const
{
    return hp <= 0;
}

void Samurai::respawn()
{
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
    switchState(SamuraiState::Idle);
}

void Samurai::queueAttack(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Q) {
        attackType = AttackVariant::Type1;
    } else if (key == sf::Keyboard::E) {
        attackType = AttackVariant::Type2;
    }
    attackQueued = true;
}

void Samurai::performAttack(AttackVariant type, std::vector<std::unique_ptr<Enemy>>& enemies)
{
    switchState(SamuraiState::Attack);
    currentFrame = 0;
    animationTimer = 0.f;
    attackTimer = 0.f;

    if (type == AttackVariant::Type1)
        sprite.setTexture(attack1Tex);
    else
        sprite.setTexture(attack2Tex);
    if (type == AttackVariant::Type1) {
        sprite.setTexture(attack1Tex);
        frameCount = 6;  // 🔁 liczba klatek dla Attack_1
    } else {
        sprite.setTexture(attack2Tex);
        frameCount = 4;  // 🔁 liczba klatek dla Attack_2
    }

    frameSize = {128, 128};
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});

    // 🗡️ ZADAWANIE OBRAŻEŃ
    for (auto& enemy : enemies) {
        if (!enemy->isDead()) {
            float dist = std::hypot(enemy->getPosition().x - sprite.getPosition().x,
                                    enemy->getPosition().y - sprite.getPosition().y);
            if (dist <= range) {
                enemy->takeDamage(damage);
                break; // tylko jeden przeciwnik na raz
            }
        }
    }
}

void Samurai::handleMovement(float deltaTime, const sf::RenderWindow& window)
{
    sf::Vector2f moveDir(0.f, 0.f);
    // ZATRZYMANIE PRZED BRĄZOWĄ STREFĄ
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u winSize = window.getSize();

    float margin = 20.f;
    float topLimit = 130.f;
    float rightLimit = winSize.x - 335.f;
    float bottomLimit = winSize.y - margin;

    pos.x = std::clamp(pos.x, margin, rightLimit);
    pos.y = std::clamp(pos.y, topLimit, bottomLimit);

    sprite.setPosition(pos);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    if (moveDir != sf::Vector2f(0.f, 0.f)) {
        moveDir /= std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        sprite.move(moveDir * speed * deltaTime);

        sprite.setScale(moveDir.x < -0.1f ? -1.f : 1.f, 1.f);
        sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);

        if (state != SamuraiState::Walk && state != SamuraiState::Attack)
            switchState(SamuraiState::Walk);
    } else if (state != SamuraiState::Idle && state != SamuraiState::Attack && state != SamuraiState::Dead) {
        switchState(SamuraiState::Idle);
    }
}

void Samurai::updateAnimation(float deltaTime)
{
    animationTimer += deltaTime;

    if (animationTimer >= frameTime) {
        animationTimer = 0.f;

        if (state == SamuraiState::Dead) {
            // Animacja śmierci nie zapętla się
            if (currentFrame < frameCount - 1)
                currentFrame++;  // dojdzie do ostatniej klatki i tam zostanie
        } else {
            // Wszystkie inne animacje zapętlane
            currentFrame = (currentFrame + 1) % frameCount;
        }

        sprite.setTextureRect({currentFrame * frameSize.x, 0, frameSize.x, frameSize.y});
    }
}

void Samurai::switchState(SamuraiState newState)
{
    state = newState;
    currentFrame = 0;
    animationTimer = 0.f;
    updateTexture();
}

void Samurai::updateTexture()
{
    switch (state) {
    case SamuraiState::Idle:
        sprite.setTexture(idleTex);
        frameCount = 6;
        break;
    case SamuraiState::Walk:
        sprite.setTexture(walkTex);
        frameCount = 6;
        break;
    case SamuraiState::Dash:
        sprite.setTexture(runTex);
        frameCount = 6;
        break;
    case SamuraiState::Attack:
        // tekstura ustawiona ręcznie
        return;
    case SamuraiState::Dead:
        sprite.setTexture(deathTex);
        frameCount = 3;
        break;
    case SamuraiState::Win:
        break;
    }

    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);
}

void Samurai::handleAttack(std::vector<std::unique_ptr<Enemy>>&, const sf::RenderWindow&) {
    // nieużywana funkcja, wymagana przez HeroBase
}
