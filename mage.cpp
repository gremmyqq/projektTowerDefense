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
    attackTimer = 0.f;
    level = 1;

    frameSize = {128, 128};
    frameTime = 0.15f;

    // Ładowanie tekstur animacji
    if (!idleTex.loadFromFile("assets/Idle.png") ||
        !walkTex.loadFromFile("assets/Walk.png") ||
        !runTex.loadFromFile("assets/Run.png") ||
        !attackTex.loadFromFile("assets/Attack_1.png") ||
        !deathTex.loadFromFile("assets/Dead.png")) {
        throw std::runtime_error("Nie można załadować tekstur maga.");
    }

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

    // 🔧 automatyczne wyjście ze stanu Attack po zakończeniu animacji
    if (state == MageState::Attack && currentFrame >= frameCount - 1 && animationTimer == 0.f)
        switchState(MageState::Idle);
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

        float moveSpeed = speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            moveSpeed *= 1.5f;
            switchState(MageState::Run);
        } else {
            switchState(MageState::Walk);
        }

        sprite.move(moveDir * moveSpeed * deltaTime);
    } else {
        switchState(MageState::Idle);
    }
}

void Mage::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                        const sf::RenderWindow& window) {
    if (!attackQueued) return;
    if (attackTimer < attackCooldown) return;


        switchState(MageState::Attack);
        currentFrame = 0;
        animationTimer = 0.f;
        attackTimer = 0.f;

        for (auto& enemy : enemies) {
            if (!enemy->isDead()) {
                float dist = std::hypot(enemy->getPosition().x - sprite.getPosition().x,
                                        enemy->getPosition().y - sprite.getPosition().y);
                if (dist <= range) {
                    enemy->takeDamage(damage);
                    std::cout << "[HIT] Zadałem obrażenia!\n";
                    break;
                }
            }
        }
        attackQueued = false;

}

void Mage::draw(sf::RenderWindow& window) {
    window.draw(sprite);

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
        sprite.setTexture(idleTex); frameCount = 6; break;
    case MageState::Walk:
        sprite.setTexture(walkTex); frameCount = 7; break;
    case MageState::Run:
        sprite.setTexture(runTex); frameCount = 8; break;
    case MageState::Attack:
        sprite.setTexture(attackTex); frameCount = 10; break;
    case MageState::Dead:
        sprite.setTexture(deathTex); frameCount = 4; break;
// użycie tej samej animacji
    }
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
}

void Mage::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameTime) {
        animationTimer = 0.f;
        if (state == MageState::Dead && currentFrame >= frameCount - 1) {
            return;
        }
        currentFrame = (currentFrame + 1) % frameCount;
        sprite.setTextureRect({currentFrame * frameSize.x, 0, frameSize.x, frameSize.y});
    }
}
