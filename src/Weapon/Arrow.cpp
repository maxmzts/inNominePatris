#include "Arrow.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

sf::Texture Arrow::arrowTexture;

Arrow::Arrow(const sf::Vector2f& position, const sf::Vector2f& direction, float speed) {
    // Cargar la textura si no se ha cargado aún
    if (arrowTexture.getSize().x == 0) {
        if (!arrowTexture.loadFromFile("./src/resources/Weapons/Arrow.png")) {
            std::cerr << "Error cargando la textura arrow.png";
        }
    }

    // Configurar el sprite con la textura
    sprite.setTexture(arrowTexture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f); // Escala de la flecha (ajustar según sea necesario)
    float angle = std::atan2(direction.y, direction.x) * 180.f / M_PI; // Convertir de radianes a grados
    sprite.setRotation(angle - 45);
    velocity = direction * speed;
}

void Arrow::update(float deltaTime) {
    // Actualizar la posición de la flecha
    sprite.move(velocity * deltaTime);
}

void Arrow::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

const sf::FloatRect Arrow::getBounds() const {
    return sprite.getGlobalBounds();
}

void Arrow::markforRemoval() {
    markedForRemoval = true;
}

bool Arrow::isMarkedForRemoval() const {
    return markedForRemoval;
}