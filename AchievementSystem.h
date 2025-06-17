#ifndef ACHIEVEMENT_SYSTEM_H
#define ACHIEVEMENT_SYSTEM_H

#include <string>
#include <unordered_map>

class AchievementSystem {
public:
    enum class Type {
        FirstKill,
        HeroBought,
        CastleUpgraded,
        TowerToLevel4,
        BossKilled
    };

    void unlock(Type type);
    bool isUnlocked(Type type) const;
    void reset();

private:
    std::unordered_map<Type, bool> unlocked;
};

#endif
