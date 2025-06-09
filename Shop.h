#ifndef SHOP_H
#define SHOP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <vector>

class Shop {
public:
    Shop();

    void setFont(const sf::Font& font);
    void setPosition(const sf::Vector2f& position);
    void setGoldPointer(int* goldPtr);
    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);

    void addItem(const std::string& name, int cost, std::function<void()> action);

private:
    struct ShopItem {
        std::string name;
        int cost;
        sf::RectangleShape button;
        sf::Text label;
        std::function<void()> onBuy;
    };

    std::vector<ShopItem> items;
    sf::Vector2f basePosition;
    const sf::Font* font;
    int* gold;
};

#endif // SHOP_H
