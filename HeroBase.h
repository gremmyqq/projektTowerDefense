#ifndef HEROBASE_H
#define HEROBASE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"

class HeroBase {
public:
    HeroBase(const sf::Vector2f& spawnPoint, const sf::Texture& texture);
    virtual ~HeroBase() = default;

    // Główne interfejsy
    virtual void update(float deltaTime,
                        const sf::RenderWindow& window,
                        std::vector<std::unique_ptr<Enemy>>& enemies) = 0;

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void takeDamage(int dmg);
    virtual bool isDead() const;
    virtual void respawn();
    virtual void queueAttack(); // nowa funkcja
    sf::Vector2f getPosition() const;

protected:
    // Wygląd
    sf::Sprite sprite;
    sf::RectangleShape hpBarBg, hpBar;

    // Statystyki
    float speed = 150.f;
    int maxHp = 100;
    int hp = 100;
    int damage = 10;
    float attackCooldown = 0.5f;
    float attackTimer = 0.f;
    int level = 1;
    float range = 50.f;

    // Pozycja spawnu
    sf::Vector2f spawnPosition;

    // Animacja
    int currentFrame = 0;
    int frameCount = 6;
    float frameTime = 0.1f;
    float animationTimer = 0.f;
    sf::Vector2i frameSize = {64, 64};

    // Dash
    bool dashing = false;
    float dashTime = 0.f;
    float dashDuration = 0.15f;
    float dashSpeed = 400.f;

    // Metody wirtualne pomocnicze (dla podklas)
    virtual void handleMovement(float deltaTime);
    virtual void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                              const sf::RenderWindow& window) = 0;
    virtual void updateAnimation(float deltaTime) = 0;

    // Pasek HP
    void updateHpBar();
    bool attackQueued = false;

};

#endif
