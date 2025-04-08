#include "Game.h"
#include "MainMenu.h"

int main() {
    // Crear el juego con una ventana de 800x600
    Game game("In Nomine Patris", 800, 600);

    // Establecer el estado inicial como MainMenu
    game.changeState(MainMenu::getInstance(800, 600));

    // Bucle principal del juego
    while (game.getWindow().isOpen()) {
        game.update();  // Actualizar el estado actual
        game.render();  // Renderizar el estado actual
    }

    return 0;
}