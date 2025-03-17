#ifndef MENUCONF_H
#define MENUCONF_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MenuConf {
public:
    MenuConf(float width, float height);
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
