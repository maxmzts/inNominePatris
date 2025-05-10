#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <SFML/Graphics.hpp>
#include "State.h"
#include "../Game.h"
#include <vector>

class PauseMenu : public State {
public:
    static PauseMenu* getInstance(float width, float height);
    void update(Game& game);
    void render(Game& game, sf::RenderWindow& window);

private:
    PauseMenu(float width, float height);
    static PauseMenu* instance;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<sf::RectangleShape> menuBackgrounds;
    std::vector<sf::Text> menuItems;
    sf::Font font;
    int selectedItemIndex;

    void moveUp();
    void moveDown();
    void handleMouseClick(Game& game, sf::RenderWindow& window); // Manejar clics del ratón
    void handleSelection(Game& game); // Manejar la selección de una opción
};

#endif