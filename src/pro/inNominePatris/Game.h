#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "State.h"

class Game {
private:
    sf::RenderWindow window; // Ventana principal del juego
    State* currentState;     // Estado actual del juego

public:
    Game(const std::string& title, int width, int height);
    ~Game();

    void update();           // Actualiza el estado actual
    void render();           // Renderiza el estado actual
    void changeState(State* newState); // Cambia al nuevo estado
    void restartGame();      // Reinicia el juego

    sf::RenderWindow& getWindow(); // Devuelve la ventana del juego
    State* getCurrentState() const { return currentState; } // Devuelve el estado actual
};

#endif // GAME_H