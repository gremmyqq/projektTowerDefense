#include "AchievementSystem.h"

void AchievementSystem::unlock(Type type) {
    unlocked[type] = true;
}

bool AchievementSystem::isUnlocked(Type type) const {
    auto it = unlocked.find(type);
    return it != unlocked.end() && it->second;
}

void AchievementSystem::reset() {
    unlocked.clear();
}
