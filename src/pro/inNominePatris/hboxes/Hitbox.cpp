#include "Hitbox.h"

Hitbox::Hitbox(const sf::Vector2f& size, const sf::Vector2f& offset, const sf::Color& color)
    : m_offset(offset)
    , m_position(0.0f, 0.0f)
    , m_active(true)
{
    // Inicializar la forma rectangular
    m_shape.setSize(size);
    m_shape.setFillColor(color);
    
    // Centrar el origen del rectángulo
    m_shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
}

Hitbox::~Hitbox() {
    // No hay recursos que liberar explícitamente
}

void Hitbox::setPosition(const sf::Vector2f& position) {
    m_position = position;
    
    // Aplicar el offset a la posición base
    sf::Vector2f finalPosition = position + m_offset;
    m_shape.setPosition(finalPosition);
}

sf::Vector2f Hitbox::getPosition() const {
    return m_position;
}

void Hitbox::setSize(const sf::Vector2f& size) {
    m_shape.setSize(size);
    
    // Actualizar el origen para mantenerlo centrado
    m_shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
}

sf::Vector2f Hitbox::getSize() const {
    return m_shape.getSize();
}

void Hitbox::setOffset(const sf::Vector2f& offset) {
    m_offset = offset;
    
    // Actualizar la posición con el nuevo offset
    setPosition(m_position);
}

sf::Vector2f Hitbox::getOffset() const {
    return m_offset;
}

void Hitbox::setColor(const sf::Color& color) {
    m_shape.setFillColor(color);
}

void Hitbox::setActive(bool active) {
    m_active = active;
    
    // Cambiar visualmente si está activo o no
    if (active) {
        // Mantener el color pero con opacidad completa
        sf::Color currentColor = m_shape.getFillColor();
        currentColor.a = 128; // Semi-transparente
        m_shape.setFillColor(currentColor);
    } else {
        // Si está inactivo, reducir la opacidad
        sf::Color currentColor = m_shape.getFillColor();
        currentColor.a = 64; // Muy transparente
        m_shape.setFillColor(currentColor);
    }
}

bool Hitbox::isActive() const {
    return m_active;
}

void Hitbox::render(sf::RenderWindow& window) {
    // Solo renderizar si está activo
    if (m_active) {
        window.draw(m_shape);
    }
}

sf::FloatRect Hitbox::getGlobalBounds() const {
    return m_shape.getGlobalBounds();
}