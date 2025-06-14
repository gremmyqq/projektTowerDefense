#ifndef TOWERARCHER_H
#define TOWERARCHER_H

#include "TowerField.h"

class TowerArcher : public TowerField {
public:
    explicit TowerArcher(const sf::Vector2f& pos);
    void attack(std::vector<std::unique_ptr<Enemy>>& enemies) override;

};

#endif
