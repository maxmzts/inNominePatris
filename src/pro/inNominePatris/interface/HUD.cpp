#include "HUD.h"
#include <iostream>

HUD::HUD(float width, float height) {
    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente para el HUD\n";
    }

    // Configurar barra de vida
    healthBarBackground.setSize(sf::Vector2f(200, 20));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50));
    healthBarBackground.setPosition(10, height - 40); // Coordenadas de pantalla

    healthBar.setSize(sf::Vector2f(200, 20));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(10, height - 40); // Coordenadas de pantalla

    // Configurar inventario
    inventoryBackground.setSize(sf::Vector2f(300, 100));
    inventoryBackground.setFillColor(sf::Color(50, 50, 50, 200));
    inventoryBackground.setPosition(10, height - 150); // Coordenadas de pantalla

    // Configurar texto del arma
    weaponText.setFont(font);
    weaponText.setCharacterSize(20);
    weaponText.setFillColor(sf::Color::White);
    weaponText.setPosition(20, height - 140); // Coordenadas de pantalla
}

void HUD::update(const Character& character) {
    // Actualizar barra de vida
    float healthPercentage = static_cast<float>(character.getHealth()) / character.getMaxHealth();
    healthPercentage = std::max(0.0f, std::min(healthPercentage, 1.0f)); // Limitar entre 0 y 1
    healthBar.setSize(sf::Vector2f(200 * healthPercentage, 20));

    // Actualizar texto del arma equipada
    Weapon* equippedWeapon = character.getEquippedWeapon();
    if (equippedWeapon) {
        weaponText.setString("Arma equipada: " + equippedWeapon->getName());
    } else {
        weaponText.setString("Sin arma equipada");
    }
}

void HUD::draw(sf::RenderWindow& window, const Character& character) {
    // Guardar la vista actual
    sf::View originalView = window.getView();

    // Restablecer la vista para usar coordenadas de pantalla
    window.setView(window.getDefaultView());

    // Dibujar barra de vida
    window.draw(healthBarBackground);
    window.draw(healthBar);

    // Dibujar inventario
    window.draw(inventoryBackground);

    // Dibujar texto del arma equipada
    window.draw(weaponText);

    // Restaurar la vista original
    window.setView(originalView);
}