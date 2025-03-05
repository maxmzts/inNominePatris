#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow &window);
    void moveUp();
    void moveDown();
    int getSelectedItem() { return selectedItemIndex; }

private:
    int selectedItemIndex;
    std::vector<sf::Text> menuItems;
    std::vector<sf::RectangleShape> menuBackgrounds;
    sf::Font font; // Fuente para los textos
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

#endif 
