#include "PauseMenu.h"
#include <iostream>

PauseMenu* PauseMenu::instance = nullptr;

PauseMenu* PauseMenu::getInstance() {
    if (!instance) {
        instance = new PauseMenu();
    }
    return instance;
}

void PauseMenu::update(Game& game) {
    std::cout << "Updating PauseMenu state...\n";
    // Lógica para pausar el juego
}

void PauseMenu::render(Game& game, sf::RenderWindow& window) {
    std::cout << "Rendering PauseMenu state...\n";
    // Renderizado del menú de pausa
}