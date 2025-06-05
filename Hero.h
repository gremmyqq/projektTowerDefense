#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Enemy.h"

// Klasa reprezentująca bohatera sterowanego przez gracza
class Hero {
public:
    // Konstruktor — ustawia pozycję spawnu i teksturę sprite’a
    Hero(const sf::Vector2f& spawnPoint, const sf::Texture& texture);

    // Aktualizacja bohatera: ruch, atak, pasek HP
    void update(float deltaTime, const sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies);

    // Rysowanie bohatera i jego paska zdrowia
    void draw(sf::RenderWindow& window);

    // Odbieranie obrażeń
    void takeDamage(int dmg);

    // Czy bohater jest martwy
    bool isDead() const;

    // Respawn — resetuje statystyki i ustawia na pozycji początkowej
    void respawn();

private:
    // Sprite bohatera
    sf::Sprite sprite;

    // Pasek zdrowia
    sf::RectangleShape hpBarBg, hpBar;

    // Statystyki
    float speed;           // Prędkość poruszania
    int maxHp;             // Maksymalne zdrowie
    int hp;                // Aktualne zdrowie
    int damage;            // Obrażenia
    float attackCooldown;  // Odstęp między atakami
    float attackTimer;     // Licznik czasu od ostatniego ataku
    int level;             // Poziom bohatera
    float range;           // Zasięg ataku

    sf::Vector2f spawnPosition; // Pozycja startowa

    // Obsługa sterowania klawiaturą
    void handleMovement(float deltaTime);

    // Obsługa ataku myszką
    void handleAttack(std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window);

    // Aktualizacja graficzna paska HP
    void updateHpBar();
};

#endif // HERO_H

