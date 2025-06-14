#ifndef EMPTYFIELD_H
#define EMPTYFIELD_H

#include "Field.h"
#include "GameEngine.h"

class EmptyField : public Field {
public:
    EmptyField(const sf::Vector2f& pos);

    void update(float, std::vector<std::unique_ptr<Enemy>>&) override {}
    void draw(sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const override;
    void handleClick(BuildType selectedType, GameEngine& engine) override;

private:
    static sf::Texture fieldTexture;
    sf::Sprite fieldSprite;

};

#endif
