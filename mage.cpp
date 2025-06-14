#include "Mage.h"
#include <cmath>

Mage::Mage(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : HeroBase(spawnPoint, texture) {
    speed = 140.f;
    maxHp = 100;
    hp = maxHp;
    damage = 25;
    range = 200.f;
    attackCooldown = 1.2f;

    // Ładowanie tekstur animacji
    idleTex.loadFromFile("assets/image.png");
    runTex.loadFromFile("assets/rmage.png");
    attackTex.loadFromFile("assets/amage.png");
    extraAttackTex.loadFromFile("assets/eamage.png");
    deathTex.loadFromFile("assets/dmage.png");

    sprite.setTexture(idleTex);
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
    sprite.setPosition(spawnPoint);
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);

}

void Mage::update(float deltaTime, const sf::RenderWindow& window,
                  std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (state == MageState::Dead) {
        updateAnimation(deltaTime);
        return;
    }


    handleMovement(deltaTime);
    attackTimer += deltaTime;
    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);
}

void Mage::handleMovement(float deltaTime) {
    sf::Vector2f moveDir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    if (moveDir.x != 0.f || moveDir.y != 0.f) {
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= len;
        sprite.move(moveDir * speed * deltaTime);
        switchState(MageState::Run);
    } else {
        switchState(MageState::Idle);
    }
}

void Mage::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                        const sf::RenderWindow& window) {
    if (attackTimer < attackCooldown)
        return;

    if (normalAttackQueued) {
        switchState(MageState::Attack);
        attackTimer = 0.f;

        for (auto& enemy : enemies) {
            if (!enemy->isDead()) {
                float dist = std::hypot(enemy->getPosition().x - sprite.getPosition().x,
                                        enemy->getPosition().y - sprite.getPosition().y);
                if (dist <= range) {
                    enemy->takeDamage(damage);
                    break;
                }
            }
        }
        normalAttackQueued = false;
    }

    if (extraAttackQueued) {
        switchState(MageState::ExtraAttack);
        attackTimer = 0.f;

        for (auto& enemy : enemies) {
            if (!enemy->isDead()) {
                float dist = std::hypot(enemy->getPosition().x - sprite.getPosition().x,
                                        enemy->getPosition().y - sprite.getPosition().y);
                if (dist <= range) {
                    enemy->takeDamage(damage * 2);
                    break;
                }
            }
        }
        extraAttackQueued = false;
    }
}



void Mage::draw(sf::RenderWindow& window) {
    // Zawsze rysuj sprite — nawet w stanie Dead
    window.draw(sprite);

    // Rysuj pasek HP tylko, gdy Mage żyje
    if (!isDead()) {
        sf::Vector2f spritePos = sprite.getPosition();
        hpBarBg.setPosition(spritePos.x - 20.f, spritePos.y - 30.f);
        hpBar.setPosition(hpBarBg.getPosition());

        window.draw(hpBarBg);
        window.draw(hpBar);
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


void Mage::switchState(MageState newState) {
    if (state == newState) return;
    state = newState;
    currentFrame = 0;
    animationTimer = 0.f;
    updateTexture();
}

void Mage::updateTexture() {
    switch (state) {
    case MageState::Idle:
        sprite.setTexture(idleTex);
        frameCount = 6;
        break;
    case MageState::Run:
        sprite.setTexture(runTex);
        frameCount = 6;
        break;
    case MageState::Attack:
        sprite.setTexture(attackTex);
        frameCount = 10;
        break;
    case MageState::ExtraAttack:
        sprite.setTexture(extraAttackTex);
        frameCount = 14;
        break;
    case MageState::Dead:
        sprite.setTexture(deathTex);
        frameCount = 17;
        break;
    }
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
}

void Mage::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= frameTime) {
        animationTimer = 0.f;

        if (state == MageState::Dead) {
            // Zatrzymaj się na ostatniej klatce
            if (currentFrame < frameCount - 1) {
                currentFrame++;
                sprite.setTextureRect({currentFrame * frameSize.x, 0, frameSize.x, frameSize.y});
            }
            // jeśli już osiągnął ostatnią klatkę — nie rób nic
        } else {
            currentFrame = (currentFrame + 1) % frameCount;
            sprite.setTextureRect({currentFrame * frameSize.x, 0, frameSize.x, frameSize.y});
        }
    }
}
void Mage::queueAttack() {
    std::cout << "Mage::queueAttack()\n";
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        std::cout << "Lewy klik wykryty!\n";
        normalAttackQueued = true;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        std::cout << "Prawy klik wykryty!\n";
        extraAttackQueued = true;
    }
}


