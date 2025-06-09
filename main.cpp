#include <SFML/Graphics.hpp>
#include "GameEngine.h"

int main() {
    // Tryb pełnoekranowy
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Tower Defense"/*, sf::Style::Fullscreen*/);

    GameEngine engine(window);
    engine.run();

    return 0;
}
