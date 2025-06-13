#ifndef MAGE_H
#define MAGE_H

#include "HeroBase.h"

class Mage : public HeroBase {
public:
    Mage(const sf::Vector2f& spawnPoint, const sf::Texture& texture);

    void update(float deltaTime,
                const sf::RenderWindow& window,
                std::vector<std::unique_ptr<Enemy>>& enemies) override;

    void draw(sf::RenderWindow& window) override;
    void takeDamage(int dmg) override;
    bool isDead() const override;
    void respawn() override;

protected:
    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies,
                      const sf::RenderWindow& window) override;

    void updateAnimation(float deltaTime) override;
};

#endif
