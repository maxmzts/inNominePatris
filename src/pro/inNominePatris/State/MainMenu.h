#ifndef MAINMENU_H
#define MAINMENU_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu : public State {
private:
    static MainMenu* instance;

    sf::Font font; // Fuente para los textos
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<sf::Text> menuItems;
    std::vector<sf::RectangleShape> menuBackgrounds;
    int selectedItemIndex;

    MainMenu(float width, float height);

public:
    static MainMenu* getInstance(float width, float height);

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;

    void moveUp();
    void moveDown();
};

#endif // MAINMENU_H