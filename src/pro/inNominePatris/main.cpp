#include "Game.h"
#include "GameEngine/GameEngine.h"
#include "MainMenu.h"
#include <iostream>

int main() {
    // Crear el juego con una ventana de 800x600
    Game game("In Nomine Patris", 800, 600);
    // GameEngine engine(game.getWindow());
    // Establecer el estado inicial como MainMenu
    game.changeState(MainMenu::getInstance(game.getEngine(), 800, 600));

    // Bucle principal del juego
    while (game.getWindow().isOpen()) {
        if (game.getCurrentState() == nullptr) {
            std::cerr << "No hay un estado válido. Cerrando la ventana...\n";
            game.getWindow().close();
            break;
        }

        game.update();  // Actualizar el estado actual
        game.render();  // Renderizar el estado actual
    }

    return 0;
}