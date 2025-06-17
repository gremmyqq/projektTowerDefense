#include "Knight.h"
#include <cstdlib> // rand
#include <cmath>
#include <iostream>
#include <stdexcept>

Knight::Knight(const sf::Vector2f& spawnPoint, const sf::Texture& texture)
    : HeroBase(spawnPoint, sf::Texture()) {

    if (!idleTex.loadFromFile("assets/Idle_KG_1.png") ||
        !walk1Tex.loadFromFile("assets/Walking_KG_1.png") ||
        !walk2Tex.loadFromFile("assets/Walking_KG_2.png") ||
        !attackTex[0].loadFromFile("assets/Attack_KG_1.png") ||
        !attackTex[1].loadFromFile("assets/Attack_KG_2.png") ||
        !attackTex[2].loadFromFile("assets/Attack_KG_3.png") ||
        !attackTex[3].loadFromFile("assets/Attack_KG_4.png") ||
        !deathTex.loadFromFile("assets/Dying_KG_2.png") ||
        !winTex.loadFromFile("assets/knight_win.png")) {
        throw std::runtime_error("Nie można załadować tekstur Knighta");
    }

    sprite.setTexture(idleTex);
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);

    speed = 120.f;
    maxHp = 300;
    hp = maxHp;
    damage = 80;
    range = 60.f;
    attackCooldown = 0.8f;
    attackTimer = 0.f;
}

void Knight::update(float deltaTime, const sf::RenderWindow& window,
                    std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (state == KnightState::Dead) {
        updateAnimation(deltaTime);
        return;
    }

    handleMovement(deltaTime, window);
    attackTimer += deltaTime;

    handleAttack(enemies, window);
    updateHpBar();
    updateAnimation(deltaTime);

    // automatyczne przejście do Idle
    if (state == KnightState::Attack && currentFrame == frameCount - 1)
        switchState(KnightState::Idle);
}

void Knight::draw(sf::RenderWindow& window) {
    if (state != KnightState::Dead) {
        hpBarBg.setPosition(sprite.getPosition().x - 20.f, sprite.getPosition().y - 60.f);
        hpBar.setPosition(hpBarBg.getPosition());
        window.draw(hpBarBg);
        window.draw(hpBar);
    }

    window.draw(sprite); // <- rysuj sprite ZAWSZE, nawet w stanie Dead

}

void Knight::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        switchState(KnightState::Dead);
    }
}

bool Knight::isDead() const {
    return hp <= 0;
}

void Knight::respawn() {
    hp = maxHp;
    level = 1;
    sprite.setPosition(spawnPosition);
    switchState(KnightState::Idle);
}

void Knight::handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                          const sf::RenderWindow& window) {
    if (!attackQueued) return;

    // Poczekaj na cooldown
    if (attackTimer < attackCooldown)
        return;

    std::cout << "[INFO] Wykonuję atak!\n";

    // losowa animacja
    int r = rand() % 4;
    sprite.setTexture(attackTex[r]);
    switchState(KnightState::Attack);
    currentFrame = 0;
    animationTimer = 0.f;
    attackTimer = 0.f;

    // zadaj obrażenia tylko JEDNEMU wrogowi w zasięgu
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

    attackQueued = false;  // zresetuj dopiero po pełnym ataku
}




void Knight::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= frameTime) {
        if (state == KnightState::Dead) {
            // Zatrzymaj animację na ostatniej klatce
            if (currentFrame < frameCount - 1) {
                currentFrame++;
                sprite.setTextureRect({
                    currentFrame * frameSize.x, 0, frameSize.x, frameSize.y
                });
            }
            // Nie resetuj currentFrame — pozostanie na ostatniej klatce
        } else {
            currentFrame = (currentFrame + 1) % frameCount;
            sprite.setTextureRect({
                currentFrame * frameSize.x, 0, frameSize.x, frameSize.y
            });
        }

        animationTimer = 0.f;
    }
}


void Knight::switchState(KnightState newState) {
    state = newState;
    currentFrame = 0;
    animationTimer = 0.f;
    updateTexture();
}
void Knight::handleMovement(float deltaTime, const sf::RenderWindow& window)
 {
    sf::Vector2f moveDir(0.f, 0.f);
     // ZATRZYMANIE PRZED BRĄZOWĄ STREFĄ
     sf::Vector2f pos = sprite.getPosition();
     sf::Vector2u winSize = window.getSize();

     // Ograniczenia wyrażone jako procenty (np. 5% marginesu po bokach)
     float marginX = winSize.x * 0.005f;
     float marginY = winSize.y * 0.04f;
     float topLimit = winSize.y * 0.10f;
     float rightLimit = winSize.x * 0.865f;
     float bottomLimit = winSize.y - marginY;

     // Przypnij pozycję do ograniczeń
     pos.x = std::clamp(pos.x, marginX, rightLimit);
     pos.y = std::clamp(pos.y, topLimit, bottomLimit);

     sprite.setPosition(pos);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir.x += 1.f;

    // Dash: aktywacja na SHIFT
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !dashing) {
        dashing = true;
        dashTime = dashDuration;
        switchState(KnightState::Dash);
    }

    if (moveDir != sf::Vector2f(0.f, 0.f)) {
        moveDir /= std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);

        float currentSpeed = dashing ? dashSpeed : speed;
        sprite.move(moveDir * currentSpeed * deltaTime);

        // Obracanie sprite'a względem kierunku ruchu
        if (moveDir.x < -0.1f)
            sprite.setScale(-1.f, 1.f);
        else if (moveDir.x > 0.1f)
            sprite.setScale(1.f, 1.f);

        sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);

        if (!dashing && state != KnightState::Walk && state != KnightState::Attack)
            switchState(KnightState::Walk);
    } else {
        if (!dashing && state != KnightState::Idle && state != KnightState::Attack && state != KnightState::Dead) {
            switchState(KnightState::Idle);
        }
    }

    // Dezaktywacja dasza po czasie
    if (dashing) {
        dashTime -= deltaTime;
        if (dashTime <= 0.f) {
            dashing = false;
            switchState(KnightState::Idle); // lub Walk jeśli nadal wciśnięto ruch
        }
    }
}
void Knight::updateTexture() {
    sf::Texture* newTexture = nullptr;

    switch (state) {
    case KnightState::Idle:
        newTexture = &idleTex;
        frameCount = 4;
        frameSize = {100, 64};  // np. 256x64 sprite sheet
        break;

    case KnightState::Walk:
        newTexture = &walk1Tex;
        frameCount = 7;
        frameSize = {100, 64};  // np. 512x64
        break;

    case KnightState::Dash:
        newTexture = &walk2Tex;
        frameCount = 8;
        frameSize = {100, 64};  // np. 512x64
        break;

    case KnightState::Attack:
        // Tekstura została wcześniej ustawiona losowo w handleAttack()
        frameCount = 6;       // np. 6 klatek w Attack_KG_X
        frameSize = {100, 64}; // upewnij się, że pasuje
        return;               // NIE nadpisuj sprite.setTexture() tutaj!
        break;

    case KnightState::Dead:
        newTexture = &deathTex;
        frameCount = 6;
        frameSize = {100, 64}; // np. 384x64
        break;

    case KnightState::Win:
        newTexture = &winTex;
        frameCount = 5;
        frameSize = {100, 64};
        break;
    }

    // ustaw teksturę tylko jeśli się zmieniła (zapobiega miganiu)
    if (newTexture && sprite.getTexture() != newTexture) {
        sprite.setTexture(*newTexture);
    }

    // ustaw pierwszy frame po zmianie stanu
    sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
}

void Knight::queueAttack() {
    attackQueued = true;
}

