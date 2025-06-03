#include <iostream>
#include <vector>
#include <memory>              // ← potrzebne do std::unique_ptr
#include "Enemy.h"
#include <SFML/Graphics.hpp>

int main()
{
    std::cout << "Test";

    // Przykład poprawnej deklaracji wektora pozycji
    std::vector<sf::Vector2f> positions;

    // Przykład listy wrogów
    std::vector<std::unique_ptr<Enemy>> enemies;

    // Zakładam, że masz coś w stylu:
    // std::vector<sf::Vector2f> path;
    // enemies.push_back(std::make_unique<BasicEnemy>(path));
    return 0;
}
