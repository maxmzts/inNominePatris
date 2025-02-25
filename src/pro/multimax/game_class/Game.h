#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

/*
    Clase para encapsular el motor de juego.
*/

class Game
{
private:
    // variables

    //window
    sf::RenderWindow* window; 
    sf::VideoMode videomode;
    sf::Event event;
    void initVariables();
    void initWindow();
public:
    //constructors and destructors
    Game();
    virtual ~Game();

    //funciones de acceso
    const bool running() const;

    //functions
    void pollEvents();
    void update();
    void render();
};

