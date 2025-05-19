#pragma once
#include <SFML/Graphics.hpp>
#include "../character/Character.h"
#include "../Weapon/Weapon.h"

class HUD {
private:
    // Atributos del HUD
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBar;
    sf::RectangleShape inventoryBackground;
    sf::Font font;
    sf::Text weaponText;
    sf::Text karmaText;
    sf::Text secondaryWeaponText;

    // Notificación de ítem recogido
    sf::RectangleShape itemNotificationBackground;
    sf::Text itemNotificationTitle;
    sf::Text itemNotificationDesc;
    bool itemNotificationActive;
    float itemNotificationDuration;
    sf::Clock deltaClock; // Reloj para medir el delta time

    // Constructor privado para Singleton
    HUD(float width, float height);

    // Evitar copia y asignación
    HUD(const HUD&) = delete;
    HUD& operator=(const HUD&) = delete;

public:
    // Método para obtener la instancia única
    static HUD& getInstance(float width = 800, float height = 600);

    // Métodos del HUD
    void update(const Character& character);
    void draw(sf::RenderWindow& window, const Character& character);
    void showItemNotification(const std::string& itemName, const std::string& itemDescription);

    // Boss Health Bar methods
    void setBossHealthBarVisibility(bool visible);
    void setBossHealth(float currentHealth, float maxHealth);

    // Boss Health Bar
    sf::RectangleShape bossHealthBarBackground;
    sf::RectangleShape bossHealthBar;
    bool bossHealthBarVisible = false;
    float bossMaxHealth = 100.f; // Default value
    float bossCurrentHealth = 100.f; // Default value
};