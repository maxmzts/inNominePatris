#ifndef CONTROLS_MENU_H
#define CONTROLS_MENU_H

#include "State.h"
#include "../GameEngine/GameEngine.h"
#include <SFML/Graphics.hpp>
#include <map>

class ControlsMenu : public State {
private:
    static ControlsMenu* instance;
    GameEngine& engine;
    sf::Texture loadingTexture;
    sf::Sprite loadingSprite;
    sf::Font font;
    sf::Text backButtonText;
    //sf::RectangleShape backButtonBackground;

    sf::Color defaultButtonTextColor;
    //sf::Color defaultButtonOutlineColor;

    sf::RectangleShape underline;
    bool underlineVisible;

    ControlsMenu(GameEngine& engine, float width, float height);

public:
    static ControlsMenu* getInstance(GameEngine& engine, float width, float height);
    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;
};

#endif