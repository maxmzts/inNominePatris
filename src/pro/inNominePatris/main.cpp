#include "GameEngine/GameEngine.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    GameEngine engine("Mi Juego", 800, 600);

    // Crear un sprite
    sf::Sprite player = engine.createSprite("resources/Weapons/sword.png", {400, 300});

    // Bucle principal
    while (engine.isRunning()) { // Método para verificar si la ventana está abierta
        engine.clear(); // Limpiar la pantalla

        // Dibujar el sprite
        engine.drawSprite(player);

        engine.display(); // Mostrar el contenido en pantalla
        engine.pollEvents(); // Manejar eventos
    }

    return 0;
}