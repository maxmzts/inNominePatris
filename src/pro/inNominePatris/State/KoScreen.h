#ifndef KOSCREEN_H
#define KOSCREEN_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

class KoScreen : public State {
public:
    static KoScreen* getInstance();
    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;

private:
    KoScreen();
    static KoScreen* instance;

    sf::Texture backgroundTexture; // Añadido: textura del fondo
    sf::Sprite backgroundSprite;   // Añadido: sprite del fondo
    sf::Font font;
    sf::Text title;
    std::vector<sf::Text> menuOptions;
    int selectedItemIndex;

    void moveUp();
    void moveDown();
};

#endif // KOSCREEN_H