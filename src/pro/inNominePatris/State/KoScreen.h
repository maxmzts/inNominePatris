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

    sf::Texture backgroundTexture; // Textura del fondo
    sf::Sprite backgroundSprite;   // Sprite del fondo
    sf::Font font;                 // Fuente para el texto
    sf::Text title;                // Título de la pantalla
    std::vector<sf::Text> menuOptions; // Opciones del menú
    std::vector<sf::RectangleShape> menuBackgrounds; // Fondos de las opciones del menú
    int selectedItemIndex;         // Índice de la opción seleccionada

    void moveUp();
    void moveDown();
    void handleMouseClick(Game& game, sf::RenderWindow& window); // Manejar clics del ratón
    void handleSelection(Game& game); // Manejar la selección de una opción
};

#endif // KOSCREEN_H