#include "GameEngine.h"
#include <iostream>

GameEngine::GameEngine(const std::string& title, int width, int height) {
    initWindow(title, width, height);
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