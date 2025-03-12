#include "HUD.h"
#include <iostream>

HUD::HUD(float width, float height) {
    // Inicialización de las barras
    healthBar.setSize(sf::Vector2f(100, 10)); // Barra de vida (tamaño inicial)
    healthBar.setFillColor(sf::Color::Green); // Color verde para la barra de vida
    healthBar.setPosition(0, height - 20); // Posición en la esquina inferior izquierda

    skillCooldownBar.setSize(sf::Vector2f(100, 10)); // Barra de cooldown (tamaño inicial)
    skillCooldownBar.setFillColor(sf::Color::Blue); // Color azul para la barra de cooldown
    skillCooldownBar.setPosition(0, height - 40); // Posición en la esquina inferior izquierda (debajo de la barra de vida)

    // Establecemos valores máximos
    maxHealth = 100.0f; // Salud máxima
    currentHealth = maxHealth; // Salud inicial
    maxCooldown = 10.0f; // Cooldown máximo de la habilidad
    currentCooldown = 0.0f; // Cooldown inicial
}

void HUD::draw(sf::RenderWindow &window, const sf::Sprite &player) {
    // Dibuja las barras en la pantalla
    window.draw(healthBar); // Dibuja la barra de vida
    window.draw(skillCooldownBar); // Dibuja la barra de cooldown

    // Actualiza la posición de la barra de vida para que siempre esté encima del jugador
    sf::Vector2f playerPos = player.getPosition();
    healthBar.setPosition(playerPos.x - 50, playerPos.y - 40); // Ajustar según el tamaño del jugador
}

void HUD::update(float health, float cooldown) {
    // Actualiza los valores de la barra de vida
    currentHealth = health;
    healthBar.setSize(sf::Vector2f(100 * (currentHealth / maxHealth), 10)); // Ajustar tamaño según la vida

    // Actualiza los valores de la barra de cooldown
    currentCooldown = cooldown;
    skillCooldownBar.setSize(sf::Vector2f(100 * (currentCooldown / maxCooldown), 10)); // Ajustar tamaño según el cooldown
}
