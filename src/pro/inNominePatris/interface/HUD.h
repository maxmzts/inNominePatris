#pragma once
#include <SFML/Graphics.hpp>
#include "../character/Character.h"
#include "../Weapon/Weapon.h"

class HUD {
private:
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBar;
    sf::RectangleShape inventoryBackground;
    sf::Font font;
    sf::Text weaponText;

public:
    HUD(float width, float height);
    void update(const Character& character);
    void draw(sf::RenderWindow& window, const Character& character);
};