#include <SFML/Graphics.hpp>
#include "EnemyRegular.h"

int main() {
    // Tryb pełnoekranowy
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "SFML Enemy Test", sf::Style::Fullscreen);

    // Wymiary okna
    float width = static_cast<float>(desktop.width);
    float height = static_cast<float>(desktop.height);

    // Ścieżka dostosowana do rozdzielczości
    std::vector<sf::Vector2f> path = {
        {width * 0.1f, height * 0.1f},
        {width * 0.3f, height * 0.2f},
        {width * 0.5f, height * 0.5f},
        {width * 0.7f, height * 0.8f},
        {width * 0.9f, height * 0.9f}
    };

    EnemyRegular enemy(path);
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
        enemy.update(deltaTime);

        window.clear(sf::Color::Black);
        enemy.draw(window);
        window.display();
    }

    return 0;
}
