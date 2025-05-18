#ifndef MAINMENU_H
#define MAINMENU_H

#include "State.h"
#include "../GameEngine/GameEngine.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu : public State {
private:
    static MainMenu* instance;

    GameEngine& engine; // Puntero al GameEngine

    sf::Font font; // Fuente para los textos
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<sf::Text> menuItems;
    std::vector<sf::RectangleShape> menuBackgrounds;
    int selectedItemIndex;

    MainMenu(GameEngine& engine, float width, float height); // Constructor modificado

    void handleMouseClick(Game& game, sf::RenderWindow& window); // Manejar clics del ratón
    void handleSelection(Game& game); // Manejar la selección de una opción

public:
    static MainMenu* getInstance(GameEngine& engine, float width, float height); // Método modificado

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;

    void moveUp();
    void moveDown();
};

#endif // MAINMENU_H