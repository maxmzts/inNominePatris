#pragma once
#include <SFML/Graphics.hpp>

class HUD {
public:
    HUD(float width, float height); // Constructor
    void draw(sf::RenderWindow &window, const sf::Sprite &player); // Dibuja el HUD
    void update(float health, float cooldown); // Actualiza el estado del HUD (vida y cooldown)

private:
    sf::RectangleShape healthBar; // Barra de vida del jugador
    sf::RectangleShape skillCooldownBar; // Barra de cooldown de habilidad

    float maxHealth; // Salud máxima
    float currentHealth; // Salud actual
    float maxCooldown; // Cooldown máximo de habilidad
    float currentCooldown; // Cooldown actual de habilidad
    sf::Vector2f playerPosition; // Posición del jugador para mover la barra de vida
};
