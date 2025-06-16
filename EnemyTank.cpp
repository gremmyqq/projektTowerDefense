#include "EnemyTank.h"


EnemyTank::EnemyTank(const std::vector<sf::Vector2f>& path)
    : Enemy(path) {
    maxHealth = 500;
    health = maxHealth;
    speed = 60;

    attackDamage = 0;

    texture.loadFromFile("assets/enemy_tank.png");
    sprite.setTexture(texture);
    frameWidth = 48;
    frameHeight = 48;
    totalFrames = 6;
    animationInterval = 0.2f;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setScale(2.f, 2.f);
    if (!path.empty()) setPosition(path[0]);

    canAttack=false;
}



void EnemyTank::update(float deltaTime) {
    if (!reachedEnd() && !isDead()) {
        moveTowardsTarget(deltaTime);
    }
    updateAnimationTank(deltaTime);
}


void EnemyTank::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(sprite);
}

void EnemyTank::updateAnimationTank(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= animationInterval) {
        animationTimer = 0.f;
        if (state == EnemyState::Attacking){
            state = EnemyState::Walking;
        }
        if (state == EnemyState::Dying) {
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                state = EnemyState::Special1; // gotowy do usunięcia
                currentFrame = 0;
                animationTimer = 0.f;
            }
        } else if (state == EnemyState::Special1){
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                state = EnemyState::Special2;
                currentFrame = 0;
                animationTimer = 0.f;
            }
        }else if (state == EnemyState::Special2){
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                markedForDeletion = true;
            }
        } else {
            currentFrame = (currentFrame + 1) % totalFrames;
        }
    }

    int row = 0;
    switch (state) {
    case EnemyState::Walking: row = direction+3; break;           // 0–3
    case EnemyState::Special1: row = direction+6; break;
    case EnemyState::Special2: row = direction+9; break;        // atak — np. 4. wiersz
    case EnemyState::Dying: row = direction; break;                     // śmierć — np. 5. wiersz
    }

    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight));
    sprite.setPosition(position);
}

