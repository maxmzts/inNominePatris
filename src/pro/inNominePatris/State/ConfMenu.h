#ifndef CONFMENU_H
#define CONFMENU_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ConfMenu : public State {
private:
    static ConfMenu* instance;

    sf::Font font; // Fuente para los textos
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<sf::Text> menuItems;
    std::vector<sf::RectangleShape> menuBackgrounds;
    int selectedItemIndex;

    ConfMenu(float width, float height);

    void handleMouseClick(Game& game, sf::RenderWindow& window); // Manejar clics del ratón
    void handleSelection(Game& game); // Manejar la selección de una opción

public:
    static ConfMenu* getInstance(float width, float height);

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;

    void moveUp();
    void moveDown();
};

#endif // CONFMENU_H