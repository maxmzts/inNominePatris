#include "GameEngine.h"
#include "TileMap.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    GameEngine engine("In Nomine Patris", 800, 600);
    TileMap tileMap;

    if (!tileMap.loadFromFile("maps/lobby.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        return -1;
    }

    while (engine.isRunning()) {
        engine.pollEvents();

        // Dibujar todo
        engine.clear();
        tileMap.draw(engine);
        engine.display();
    }

    return 0;
}