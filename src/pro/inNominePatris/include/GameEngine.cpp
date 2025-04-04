#include "GameEngine.h"
#include <iostream>

// Constructor
GameEngine::GameEngine(const std::string& title, int width, int height)
    : window(sf::VideoMode(width, height), title) {
    window.setFramerateLimit(60); // Limitar a 60 FPS
}

// Destructor
GameEngine::~GameEngine() {
    if (window.isOpen()) {
        window.close();
    }
}

// Método para ejecutar el bucle principal
void GameEngine::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Limpiar la pantalla
        clear();

        // Dibujar todos los sprites
        for (const auto& sprite : spritesToDraw) {
            window.draw(sprite);
        }

        // Mostrar el contenido en pantalla
        display();

        // Limpiar la lista de sprites para el próximo frame
        spritesToDraw.clear();
    }
}

// Dibujar un sprite
void GameEngine::drawSprite(const sf::Sprite& sprite) {
    spritesToDraw.push_back(sprite);
}

// Limpiar la pantalla
void GameEngine::clear() {
    window.clear(sf::Color::Black); // Fondo negro
}

// Mostrar el contenido en pantalla
void GameEngine::display() {
    window.display();
}

// Verificar si la ventana está abierta
bool GameEngine::isWindowOpen() const {
    return window.isOpen();
}

// Obtener la ventana (si es necesario)
sf::RenderWindow& GameEngine::getWindow() {
    return window;
}

sf::Sprite GameEngine::createSprite(const std::string& texturePath, const sf::Vector2f& position) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(texturePath)) {
        std::cerr << "Error cargando la textura: " << texturePath << std::endl;
        delete texture;
        return sf::Sprite(); // Devuelve un sprite vacío si falla
    }

    // Crear el sprite y configurar la textura
    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(position);

    return sprite;
}