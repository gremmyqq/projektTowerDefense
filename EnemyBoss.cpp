#include "EnemyBoss.h"
#include "HeroBase.h"
#include "GameEngine.h"
#include <cmath>
#include <iostream>

EnemyBoss::EnemyBoss(const std::vector<sf::Vector2f>& path)
    : Enemy(path)
{
    maxHealth = 800;
    health = maxHealth;
    speed = 100;
    attackRange = 200.f;
    attackDamage = 1800;

    texture.loadFromFile("assets/enemy_boss.png");
    sprite.setTexture(texture);
    frameWidth = 96;
    frameHeight = 96;
    totalFrames = 6;
    animationInterval = 0.2f;

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setScale(2.f, 2.f);

    if (!path.empty()) {
        position = path[0];
    }
}

void EnemyBoss::update(float deltaTime) {
    if (state == EnemyState::Dying) {
        updateAnimation(deltaTime);
        return;
    }

    phaseTimer += deltaTime;

    if (phase == BossPhase::DashingAttack) {
        float moveStep = dashSpeed * deltaTime;
        position += dashDirection * moveStep;
        dashedDistance += moveStep;
        attackTimer += deltaTime;

        if (targetHero && !heroWasHit) {
            float distToHero = std::hypot(
                targetHero->getPosition().x - position.x,
                targetHero->getPosition().y - position.y
                );
            if (distToHero <= 30.f) {
                targetHero->takeDamage(attackDamage);
                heroWasHit = true;
            }
        }

        if (dashedDistance >= dashDistance || attackTimer >= minAttackDuration) {
            phase = BossPhase::Normal;
            state = EnemyState::Walking;
            attackTimer = 0.f;
            dashedDistance = 0.f;
        }

    } else if (phase == BossPhase::Charging) {
        if (phaseTimer >= chargeDuration) {
            phase = BossPhase::DashingAttack;
            state = EnemyState::Attacking;
            phaseTimer = 0.f;
            dashedDistance = 0.f;
            attackTimer = 0.f;
        }

    } else {
        moveTowardsTarget(deltaTime);
    }
    if (!path.empty()) {
        float distToEnd = std::hypot(
            path.back().x - position.x,
            path.back().y - position.y
            );
        if (distToEnd < 30.f && state != EnemyState::Dying) {
            GameEngine::instance->getCastle().takeDamage(200); // lub inna wartość

            die();
        }
    }




    updateAnimation(deltaTime);
}

void EnemyBoss::updateAgainstHero(float deltaTime, HeroBase& hero) {
    if (state == EnemyState::Dying || phase != BossPhase::Normal) return;

    sf::Vector2f toHero = hero.getPosition() - position;
    float dist = std::sqrt(toHero.x * toHero.x + toHero.y * toHero.y);

    if (dist <= attackRange) {
        phase = BossPhase::Charging;
        phaseTimer = 0.f;
        dashDirection = toHero / dist;
        state = EnemyState::Attacking;
        currentFrame = 0;
        targetHero = &hero;
        heroWasHit = false;

        updateSpriteDirection(toHero);
    }
}

void EnemyBoss::onAttackAnimationFinished() {
    // (opcjonalnie) dodatkowa logika po zakończeniu animacji ataku
}

void EnemyBoss::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void EnemyBoss::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.f;

        if (state == EnemyState::Dying) {
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                markedForDeletion = true;
            }
        } else if (state == EnemyState::Attacking) {
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            } else {
                onAttackAnimationFinished();
                currentFrame = 0;

                if (phase == BossPhase::Charging) {
                    // zakończ charging -> dash
                    phase = BossPhase::DashingAttack;
                    phaseTimer = 0.f;
                    dashedDistance = 0.f;
                    attackTimer = 0.f;
                    state = EnemyState::Attacking;
                } else {
                    state = EnemyState::Walking;
                }
            }
        } else {
            currentFrame = (currentFrame + 1) % totalFrames;
        }
    }

    int row = 0;

    if (phase == BossPhase::Charging) {
        row = direction + 9;  // specjalny rząd animacji ładowania
    } else {
        switch (state) {
        case EnemyState::Walking:  row = direction + 6; break;
        case EnemyState::Attacking: row = direction; break;
        case EnemyState::Dying:    row = direction + 3; break;
        default:                   row = direction +3; break;

        }
    }


    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight));
    sprite.setPosition(position);
}
