// GameEngine.cpp
#include "GameEngine.h"
#include <iostream>

GameEngine::GameEngine(const std::string& title, int width, int height) {
    initWindow(title, width, height);
    view.setSize(static_cast<float>(width), static_cast<float>(height));
    view.setCenter(static_cast<float>(width) / 2, static_cast<float>(height) / 2);
    window.setView(view);
}

GameEngine::~GameEngine() {
    if (window.isOpen()) {
        window.close();
    }
}

void GameEngine::initWindow(const std::string& title, int width, int height) {
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(60);
}

void GameEngine::clear() {
    window.clear(sf::Color::Black);
}

void GameEngine::display() {
    window.display();
}

void GameEngine::drawSprite(const sf::Sprite& sprite) {
    window.draw(sprite);
}

sf::Sprite GameEngine::createSprite(const std::string& texturePath, const sf::Vector2f& position) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(texturePath)) {
        std::cerr << "Error cargando la textura: " << texturePath << std::endl;
        delete texture;
        return sf::Sprite();
    }

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(position);
    return sprite;
}

bool GameEngine::isRunning() const {
    return window.isOpen();
}

void GameEngine::pollEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

sf::Texture& GameEngine::loadTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        throw std::runtime_error("Error cargando textura: " + path);
    }

    textures[path] = std::move(texture);
    return textures[path];
}

void GameEngine::drawVertices(const sf::VertexArray& vertices, const sf::Texture& texture, const sf::Transform& transform) {
    sf::RenderStates states;
    states.texture = &texture;
    states.transform = transform;
    window.draw(vertices, states);
}

sf::Vector2f GameEngine::getMousePosition() const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

sf::RenderWindow& GameEngine::getWindow() {
    return window;
}

// --- NUEVO ---
void GameEngine::setViewCenter(const sf::Vector2f& center) {
    view.setCenter(center);
    window.setView(view);
}

void GameEngine::resetView() {
    window.setView(view);
}
