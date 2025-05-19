#include "KoScreen.h"
#include "../Game.h"
#include <iostream>

KoScreen* KoScreen::instance = nullptr;

KoScreen* KoScreen::getInstance() {
    if (!instance) {
        instance = new KoScreen();
    }
    return instance;
}

void KoScreen::update(Game& game) {
    std::cout << "Updating KoScreen state...\n";
    // Lógica para manejar la pantalla de KO
}

void KoScreen::render(Game& game, sf::RenderWindow& window) {
    GameEngine& engine = game.getEngine();

    // Lógica para renderizar elementos específicos de la pantalla de KO
    std::cout << "Rendering KoScreen state...\n";
}