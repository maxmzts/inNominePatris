#include "Game.h"
#include "InGame.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "ControlsMenu.h"
#include "KoScreen.h"
#include <iostream>

Game::Game(const std::string& title, int width, int height)
    : currentState(nullptr), window(sf::VideoMode(width, height), title), engine(window) {}

Game::~Game() {
    // Limpia el estado actual si existe
    if (currentState) {
        // If the current state is not a singleton, delete it
        if (currentState != MainMenu::getInstance(engine, 800, 600) && 
            dynamic_cast<InGame*>(currentState) == nullptr &&
            currentState != ControlsMenu::getInstance(engine, 800, 600) &&
            currentState != PauseMenu::getInstance(800, 600) &&
            currentState != KoScreen::getInstance()) {
            delete currentState;
        }
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
        // std::cout << "El estado actual ya es el mismo. No se realiza el cambio.\n";
        return;
    }

    if (currentState) {
        // std::cout << "Eliminando estado actual\n";
        // If the current state is not a singleton, delete it
        if (currentState != MainMenu::getInstance(engine, 800, 600) && 
            dynamic_cast<InGame*>(currentState) == nullptr &&
            currentState != ControlsMenu::getInstance(engine, 800, 600) &&
            currentState != PauseMenu::getInstance(800, 600) &&
            currentState != KoScreen::getInstance()) {
            delete currentState;
            // std::cout << "Estado actual eliminado\n";
        }
    }

    currentState = newState; // Cambia al nuevo estado
    // std::cout << "Nuevo estado asignado\n";
}

void Game::restartGame() {
    // std::cout << "Reiniciando el juego...\n";
    // Implementar la lógica para reiniciar el juego
    // Por ejemplo, volver al estado inicial (MainMenu)
}

sf::RenderWindow& Game::getWindow() {
    return window; // Devuelve la ventana del juego
}

GameEngine& Game::getEngine() {
    return engine; // Devuelve el motor del juego
}