#include "Shop.h"

Shop::Shop() {
    background.setSize({250.f, 300.f});
    background.setFillColor(sf::Color(50, 50, 50, 220));
}

void Shop::setFont(const sf::Font& f) {
    font = &f;
}

void Shop::setPosition(const sf::Vector2f& position) {
    basePosition = position;
    background.setPosition(position);
}

void Shop::setGoldPointer(int* goldPtr) {
    gold = goldPtr;
}

void Shop::addItem(const std::string& name, int cost, std::function<void()> action) {
    ShopItem item;
    item.name = name;
    item.cost = cost;
    item.onBuy = std::move(action);

    item.button.setSize({220.f, 30.f});
    item.button.setFillColor(sf::Color(100, 100, 200));
    item.button.setPosition(basePosition.x + 15.f, basePosition.y + 15.f + static_cast<float>(items.size()) * 40.f);

    item.label.setFont(*font);
    item.label.setString(name + " (" + std::to_string(cost) + ")");
    item.label.setCharacterSize(16);
    item.label.setFillColor(sf::Color::White);
    item.label.setPosition(item.button.getPosition().x + 10.f, item.button.getPosition().y + 5.f);

    items.push_back(item);
}

void Shop::handleClick(const sf::Vector2f& mousePos) {
    if (!visible) return;

    for (auto& item : items) {
        if (item.button.getGlobalBounds().contains(mousePos)) {
            if (gold && *gold >= item.cost) {
                *gold -= item.cost;
                item.onBuy();
            }
        }
    }
}

void Shop::draw(sf::RenderWindow& window) {
    if (!visible) return;

    window.draw(background);
    for (auto& item : items) {
        window.draw(item.button);
        window.draw(item.label);
    }
}

void Shop::toggleVisible() {
    visible = !visible;
}

bool Shop::isVisible() const {
    return visible;
}
