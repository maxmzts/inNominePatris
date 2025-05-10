#include "Game.h"
#include "InGame.h"
#include <iostream>

Game::Game(const std::string& title, int width, int height)
    : currentState(nullptr), window(sf::VideoMode(width, height), title), engine(window) {}

Game::~Game() {
    // Limpia el estado actual si existe
    if (currentState) {
        delete currentState;
        // currentState = nullptr;
    }
}

void Game::update() {
    if (currentState) {
        currentState->update(*this); // Llama a la función update del estado actual
    }
}

void Game::render() {
    engine.clear(); // Limpia la ventana
    if (currentState) {
        currentState->render(*this, engine.getWindow()); // Llama a la función render del estado actual
    }
    engine.display(); // Muestra el contenido renderizado
}

void Game::changeState(State* newState) {
    if (currentState == newState) {
        std::cout << "El estado actual ya es el mismo. No se realiza el cambio.\n";
        return;
    }
    if (currentState) {
        std::cout << "Eliminando estado actual\n";

        // Si el estado actual no es un singleton, elimínalo
        if (dynamic_cast<InGame*>(currentState) == nullptr) {
            delete currentState;
        }
    }
    currentState = newState; // Cambia al nuevo estado
    std::cout << "Nuevo estado asignado\n";
}

void Game::restartGame() {
    std::cout << "Reiniciando el juego...\n";
    // Implementar la lógica para reiniciar el juego
    // Por ejemplo, volver al estado inicial (MainMenu)
}

sf::RenderWindow& Game::getWindow() {
    return window; // Devuelve la ventana del juego
}

GameEngine& Game::getEngine() {
    return engine; // Devuelve el motor del juego
}