#ifndef ENEMYBOSS_H
#define ENEMYBOSS_H

#include "Enemy.h"


class EnemyBoss : public Enemy {
public:
    EnemyBoss(const std::vector<sf::Vector2f>& path);

    void update(float deltaTime) override;
    void updateAgainstHero(float deltaTime, HeroBase& hero) override;
    void onAttackAnimationFinished() override;
    void draw(sf::RenderWindow& window) override;
    bool isCharging() const {
        return phase == BossPhase::Charging;
    }



private:
    enum class BossPhase {
        Normal,
        Charging,
        DashingAttack
    } phase = BossPhase::Normal;

    float phaseTimer = 0.f;
    float chargeDuration = 6 * 0.2f;  // 6 klatek × 0.2s
    float dashDuration = 6 * 0.2f;    // 6 klatek
    float dashSpeed = 600.f;
    float dashDistance = 250.f;
    float dashedDistance = 0.f;
    sf::Vector2f dashDirection;
    float attackTimer = 0.f;
    float minAttackDuration = 6 * 0.2f; // 6 klatek

    void updateAnimation(float deltaTime) override;

    HeroBase* targetHero = nullptr;
    bool heroWasHit = false;

};

#endif // ENEMYBOSS_H
