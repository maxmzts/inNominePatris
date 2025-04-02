// detectedArea.cpp
#include "DetectedArea.h"

// Constructor para círculos
DetectedArea::DetectedArea(float radius) {
    m_shape = std::make_unique<sf::CircleShape>(radius);
}

// Métodos de acceso a la forma
sf::Shape& DetectedArea::getShape() { 
    return *m_shape; 
}

const sf::Shape& DetectedArea::getShape() const { 
    return *m_shape; 
}

// Implementaciones de los métodos delegados
void DetectedArea::setPosition(const sf::Vector2f& position) { 
    m_shape->setPosition(position); 
}

void DetectedArea::setPosition(float x, float y) { 
    m_shape->setPosition(x, y); 
}

sf::Vector2f DetectedArea::getPosition() const { 
    return m_shape->getPosition(); 
}

void DetectedArea::setOrigin(const sf::Vector2f& position) { 
    m_shape->setOrigin(position); 
}

void DetectedArea::setOrigin(float x, float y) { 
    m_shape->setOrigin(x, y); 
}

void DetectedArea::setFillColor(const sf::Color& color) { 
    m_shape->setFillColor(color); 
}

sf::FloatRect DetectedArea::getGlobalBounds() const { 
    return m_shape->getGlobalBounds(); 
}

sf::FloatRect DetectedArea::getLocalBounds() const { 
    return m_shape->getLocalBounds(); 
}

// Método para dibujar
void DetectedArea::draw(sf::RenderTarget& target) const {
    target.draw(*m_shape);
}