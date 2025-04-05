#include "GameEngine/GameEngine.h"
#include "TileMap/TileMap.h"
#include "Character/Character.h"
#include "Weapon/Sword.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Crear el motor del juego
    GameEngine engine("In Nomine Patris", 800, 600);

    // Cargar el mapa
    TileMap tileMap;
    if (!tileMap.loadFromFile("maps/lobby.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        return -1;
    }

    // Crear el personaje principal
    Character player("resources/sprites.png");

    // Crear una espada y equipársela al personaje
    Sword* sword = new Sword(&engine); // Crear la espada
    player.addWeapon(sword);         // Equipar la espada al personaje
    player.equipWeapon();            // Equipar la espada

    // Ciclo principal del juego
    while (engine.isRunning()) {
        engine.pollEvents(); // Manejar eventos

        // Actualizar el personaje y otros elementos del juego
        player.update(tileMap);

        // Dibujar todo en la ventana
        engine.clear();
        tileMap.draw(engine);
        player.draw(engine);
        engine.display();
    }

    // Liberar memoria de la espada
    delete sword;

    return 0;
}