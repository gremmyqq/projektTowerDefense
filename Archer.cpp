#include "Archer.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

Archer::Archer(const sf::Vector2f& spawnPoint, const sf::Texture&)
    : HeroBase(spawnPoint, sf::Texture()),
    attackCooldown(0.6f),
    attackTimer(0.f),
    attackQueued(false),
    range(300.f),
    damage(30),
    speed(160.f),
    spawnPosition(spawnPoint),
    maxHp(100),
    hp(maxHp),
    state(ArcherState::Idle) {

    if (!idleTex.loadFromFile("assets/archer/Idle.png") ||
        !runTex.loadFromFile("assets/archer/Run.png") ||
        !walkTex.loadFromFile("assets/archer/Walk.png") ||
        !attackTex.loadFromFile("assets/archer/Shot_1.png") ||
        !deathTex.loadFromFile("assets/archer/Dead.png") ||
        !arrowTexture.loadFromFile("assets/archer/Arrow.png")) {
        throw std::runtime_error("Nie mozna zaladowac tekstur Archera");
    }

    switchState(ArcherState::Idle);

    hpBarBg.setSize({40, 5});
    hpBarBg.setFillColor(sf::Color::Red);
    hpBar.setSize({40, 5});
    hpBar.setFillColor(sf::Color::Green);
}

void Archer::update(float deltaTime, const sf::RenderWindow& window,
                    std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (state == ArcherState::Dead) {
        updateAnimation(deltaTime);
        return;
    }

    handleMovement(deltaTime);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);
    if (state == ArcherState::Attack && currentFrame == frameCount - 1) {
        switchState(ArcherState::Idle);  // przejście z Attack do Idle po animacji
    }

    for (auto& arrow : arrows) {
        for (auto& enemy : enemies) {
            if (!enemy->isDead() && arrow.checkCollision(*enemy)) {
                enemy->takeDamage(damage);
                arrow.markForRemoval();
                break; // strzała trafia tylko raz
            }
        }
    }

    for (auto& arrow : arrows)
        arrow.update(deltaTime);
    arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
                                [&](Arrow& arrow) {
                                    return arrow.isOffScreen(window) || arrow.isMarked();
                                }), arrows.end());

}

void Archer::draw(sf::RenderWindow& window) {
    if (!isDead()) {
        window.draw(sprite);
        hpBarBg.setPosition(sprite.getPosition().x + 38.f, sprite.getPosition().y + 30.f);


        hpBar.setPosition(hpBarBg.getPosition());
        window.draw(hpBarBg);
        window.draw(hpBar);
    }
    for (auto& arrow : arrows)
        arrow.draw(window);
}

void Archer::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        switchState(ArcherState::Dead);
    }
}

bool Archer::isDead() const {
    return hp <= 0;
}

void Archer::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
    switchState(ArcherState::Idle);
}

void Archer::queueAttack() {
    attackQueued = true;
}

void Archer::queueExtraAttack() {
    // Placeholder - mozesz dodac np. multi-shot lub aoe
}

void Archer::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                          const sf::RenderWindow& window) {
    if (!attackQueued || attackTimer < attackCooldown)
        return;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sprite.setTexture(attackTex);
    switchState(ArcherState::Attack);
    currentFrame = 0;
    animationTimer = 0.f;
    attackTimer = 0.f;

    sf::Vector2f offset(100.f, 90.f); // dopasuj do pozycji luku
    arrows.emplace_back(sprite.getPosition() + offset, mousePos, arrowTexture);

    attackQueued = false;
}

void Archer::handleMovement(float deltaTime) {
    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !dashing) {
        dashing = true;
        dashTime = dashDuration;
        switchState(ArcherState::Dash);
    }

    if (moveDir != sf::Vector2f(0.f, 0.f)) {
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= len;

        float currentSpeed = dashing ? dashSpeed : speed;
        sprite.move(moveDir * currentSpeed * deltaTime);

        if (!dashing && state != ArcherState::Walk && state != ArcherState::Attack)
            switchState(ArcherState::Walk);
    } else {
        if (!dashing && state != ArcherState::Idle && state != ArcherState::Attack && state != ArcherState::Dead)
            switchState(ArcherState::Idle);
    }

    if (dashing) {
        dashTime -= deltaTime;
        if (dashTime <= 0.f) {
            dashing = false;
            switchState(ArcherState::Idle);
        }
    }
}

void Archer::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= frameTime) {
        if (state == ArcherState::Dead) {
            if (currentFrame < frameCount - 1)
                currentFrame++;
        } else {
            currentFrame = (currentFrame + 1) % frameCount;
        }

        sprite.setTextureRect({currentFrame * frameSize.x, 0, frameSize.x, frameSize.y});
        animationTimer = 0.f;
    }
}

void Archer::switchState(ArcherState newState) {
    state = newState;
    currentFrame = 0;
    animationTimer = 0.f;
    updateTexture();
}

void Archer::updateTexture() {
    sf::Texture* tex = nullptr;

    switch (state) {
    case ArcherState::Idle:
        tex = &idleTex;
        frameCount = 6;
        frameSize = {128, 128};
        break;
    case ArcherState::Walk:
        tex = &walkTex;
        frameCount = 8;
        frameSize = {128, 128};
        break;
    case ArcherState::Dash:
        tex = &runTex;
        frameCount = 6;
        frameSize = {128, 128};
        break;
    case ArcherState::Attack:
        frameCount = 14;
        frameSize = {128, 128};
        return;
    case ArcherState::Dead:
        tex = &deathTex;
        frameCount = 3;
        frameSize = {128, 128};
        break;
    case ArcherState::Win:
        tex = &winTex;
        frameCount = 5;
        frameSize = {128, 128};
        break;
    }

    if (tex && sprite.getTexture() != tex)
        sprite.setTexture(*tex);
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
}

void Archer::updateHpBar() {
    float ratio = static_cast<float>(hp) / maxHp;
    hpBar.setSize({40.f * ratio, 5.f});
}
