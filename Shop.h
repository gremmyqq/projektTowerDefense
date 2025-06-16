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

    void addItem(const std::string& name, int cost, std::function<void()> action);
    void handleClick(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window);

    void toggleVisible();
    void toggleVisible(bool show);

    bool isVisible() const;

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
    const sf::Font* font = nullptr;
    int* gold = nullptr;

    sf::RectangleShape background;
    bool visible = false;
};

#endif // SHOP_H
