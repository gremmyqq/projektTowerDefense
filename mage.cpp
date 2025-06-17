#include "Mage.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Mage::Mage(const sf::Vector2f& spawnPoint, const sf::Texture&)
    : HeroBase(spawnPoint, sf::Texture()),
    state(MageState::Idle),
    attackCooldown(0.8f),
    attackTimer(0.f),
    attackQueued(false),
    range(350.f),
    damage(45),
    speed(140.f),
    spawnPosition(spawnPoint) {

    if (!idleTex.loadFromFile("assets/mage/Idle.png") ||
        !walkTex.loadFromFile("assets/mage/Walk.png") ||
        !runTex.loadFromFile("assets/mage/Run.png") ||
        !attackTex.loadFromFile("assets/mage/Attack_3.png") ||
        !deathTex.loadFromFile("assets/mage/Dead.png")) {
        throw std::runtime_error("Nie można załadować tekstur Mage’a");
    }

    if (!fireballTex.loadFromFile("assets/mage/FB500-3.png")) {
        throw std::runtime_error("Nie można załadować fireballa");
    }

    maxHp = 100;
    hp = maxHp;
    frameSize = {128, 128};
    switchState(MageState::Idle);
}

void Mage::update(float deltaTime, const sf::RenderWindow& window,
                  std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (state == MageState::Dead) {
        updateAnimation(deltaTime);
        return;
    }

    handleMovement(deltaTime, window);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);

    if (state == MageState::Attack && currentFrame == frameCount - 1) {
        switchState(MageState::Idle);
    }

    for (auto& spell : activeSpells) {
        spell->update(deltaTime);
        for (auto& enemy : enemies) {
            if (!enemy->isDead() && spell->checkCollision(*enemy)) {
                enemy->takeDamage(100);
            }
        }
    }

    activeSpells.erase(std::remove_if(activeSpells.begin(), activeSpells.end(),
                                      [](const std::unique_ptr<Spell>& s) { return s->isMarked(); }),
                       activeSpells.end());
}

void Mage::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hpBarBg);
    window.draw(hpBar);
    for (auto& spell : activeSpells) {
        spell->draw(window);
    }
}

void Mage::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        switchState(MageState::Dead);
    }
}

bool Mage::isDead() const {
    return hp <= 0;
}

void Mage::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
    switchState(MageState::Idle);
}

void Mage::queueAttack() {
    attackQueued = true;  // Q = melee
}

void Mage::queueExtraAttack() {
    fireballQueued = true;  // E = fireball
}


void Mage::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    if ((attackQueued || fireballQueued) && attackTimer >= attackCooldown) {
        switchState(MageState::Attack);
        currentFrame = 0;
        animationTimer = 0.f;
        attackTimer = 0.f;

        if (attackQueued) {
            // Atak wręcz: szukamy najbliższego wroga w zasięgu melee (np. 80 px)
            float meleeRange = 80.f;
            for (auto& enemy : enemies) {
                if (enemy->isDead()) continue;

                float dx = enemy->getPosition().x - getPosition().x;
                float dy = enemy->getPosition().y - getPosition().y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist <= meleeRange) {
                    enemy->takeDamage(damage);
                    break;
                }
            }
        }

        if (fireballQueued) {
            // Fireball: szukamy najbliższego wroga w zasięgu dystansowym
            Enemy* target = nullptr;
            float minDist = 100000.f;

            for (auto& enemy : enemies) {
                if (enemy->isDead()) continue;

                float dx = enemy->getPosition().x - getPosition().x;
                float dy = enemy->getPosition().y - getPosition().y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < minDist) {
                    minDist = dist;
                    target = enemy.get();
                }
            }

            if (target) {
                activeSpells.emplace_back(std::make_unique<Spell>(
                    getPosition(), target->getPosition(), fireballTex));
            }
        }

        attackQueued = false;
        fireballQueued = false;
    }
}


void Mage::handleMovement(float deltaTime, const sf::RenderWindow& window) {
    sf::Vector2f moveDir(0.f, 0.f);
    // ZATRZYMANIE PRZED BRĄZOWĄ STREFĄ
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u winSize = window.getSize();

    float margin = 25.f;
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

        if (state != MageState::Walk && state != MageState::Attack)
            switchState(MageState::Walk);
    } else if (state != MageState::Idle && state != MageState::Attack && state != MageState::Dead) {
        switchState(MageState::Idle);
    }
}

void Mage::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameTime) {
        if (state == MageState::Dead) {
            if (currentFrame < frameCount - 1)
                currentFrame++;
        } else {
            currentFrame = (currentFrame + 1) % frameCount;
        }

        sprite.setTextureRect({currentFrame * frameSize.x, 0, frameSize.x, frameSize.y});
        animationTimer = 0.f;
    }
}

void Mage::switchState(MageState newState) {
    state = newState;
    currentFrame = 0;
    animationTimer = 0.f;
    updateTexture();
}

void Mage::updateTexture() {
    sf::Texture* tex = nullptr;

    switch (state) {
    case MageState::Idle:
        tex = &idleTex; frameCount = 6; break;
    case MageState::Walk:
        tex = &walkTex; frameCount = 7; break;
    case MageState::Dash:
        tex = &runTex; frameCount = 6; break;
    case MageState::Attack:
        tex = &attackTex; frameCount = 7; break;
    case MageState::Dead:
        tex = &deathTex; frameCount = 4; break;
    case MageState::Win:
        return;
    }

    if (tex && sprite.getTexture() != tex)
        sprite.setTexture(*tex);

    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);
}
