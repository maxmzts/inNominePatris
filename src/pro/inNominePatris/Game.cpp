#include "Game.h"
#include <iostream>

Game::Game(const std::string& title, int width, int height)
    : currentState(nullptr), window(sf::VideoMode(width, height), title) {}

Game::~Game() {
    // Limpia el estado actual si existe
    if (currentState) {
        delete currentState;
        currentState = nullptr;
    }
}

void Game::update() {
    if (currentState) {
        currentState->update(*this); // Llama a la función update del estado actual
    }
}

void Game::render() {
    window.clear(); // Limpia la ventana
    if (currentState) {
        currentState->render(*this, window); // Llama a la función render del estado actual
    }
    window.display(); // Muestra el contenido renderizado
}

void Game::changeState(State* newState) {
    if (currentState) {
        delete currentState; // Limpia el estado actual
    }
    currentState = newState; // Cambia al nuevo estado
}

void Game::restartGame() {
    std::cout << "Reiniciando el juego...\n";
    // Implementar la lógica para reiniciar el juego
    // Por ejemplo, volver al estado inicial (MainMenu)
}

sf::RenderWindow& Game::getWindow() {
    return window; // Devuelve la ventana del juego
}