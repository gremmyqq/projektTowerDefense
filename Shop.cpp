#include "Shop.h"

Shop::Shop() : font(nullptr), gold(nullptr) {}

void Shop::setFont(const sf::Font& f) {
    font = &f;
}

void Shop::setPosition(const sf::Vector2f& pos) {
    basePosition = pos;
}

void Shop::setGoldPointer(int* goldPtr) {
    gold = goldPtr;
}

void Shop::addItem(const std::string& name, int cost, std::function<void()> action) {
    ShopItem item;
    item.name = name;
    item.cost = cost;
    item.onBuy = action;

    item.button.setSize({180.f, 40.f});
    item.button.setFillColor(sf::Color(150, 150, 150));
    item.button.setOutlineThickness(2.f);
    item.button.setOutlineColor(sf::Color::Black);
    item.button.setPosition(basePosition.x, basePosition.y + items.size() * 50.f);

    item.label.setFont(*font);
    item.label.setString(name + " ($" + std::to_string(cost) + ")");
    item.label.setCharacterSize(16);
    item.label.setFillColor(sf::Color::White);
    item.label.setPosition(item.button.getPosition().x + 5, item.button.getPosition().y + 8);
    items.push_back(item);
}

void Shop::draw(sf::RenderWindow& window) {
    for (const auto& item : items) {
        window.draw(item.button);
        window.draw(item.label);
    }
}

void Shop::handleClick(const sf::Vector2f& mousePos) {
    if (!gold) return;

    for (auto& item : items) {
        if (item.button.getGlobalBounds().contains(mousePos)) {
            if (*gold >= item.cost) {
                *gold -= item.cost;
                item.onBuy();
            }
        }
    }
}

void Shop::toggleVisible(bool show) {
    visible = show;
}

void Shop::toggleVisible() {
    visible = !visible;
}

bool Shop::isVisible() const {
    return visible;
}
