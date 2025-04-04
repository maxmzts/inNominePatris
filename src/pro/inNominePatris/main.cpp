#include "GameEngine.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Crear el motor de juego
    GameEngine engine("In Nomine Patris", 800, 600);

    // Crear un sprite usando el motor
    sf::Sprite sprite = engine.createSprite("resources/sprites.png", {400, 300});

    // Bucle principal
    while (engine.isWindowOpen()) {
        // Dibujar el sprite
        engine.drawSprite(sprite);

        // Ejecutar el motor
        engine.run();
    }

    return 0;
}