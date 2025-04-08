#include "KoScreen.h"
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
    std::cout << "Rendering KoScreen state...\n";
    // Renderizado de la pantalla de KO
}