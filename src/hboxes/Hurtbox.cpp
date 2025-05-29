#include "Hurtbox.h"

Hurtbox::Hurtbox(const sf::Vector2f& size, const sf::Vector2f& offset, const sf::Color& color)
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

Hurtbox::~Hurtbox() {
    // No hay recursos que liberar explícitamente
}

void Hurtbox::setPosition(const sf::Vector2f& position) {
    m_position = position;
    
    // Aplicar el offset a la posición base
    sf::Vector2f finalPosition = position + m_offset;
    m_shape.setPosition(finalPosition);
}

sf::Vector2f Hurtbox::getPosition() const {
    return m_position;
}

void Hurtbox::setSize(const sf::Vector2f& size) {
    m_shape.setSize(size);
    
    // Actualizar el origen para mantenerlo centrado
    m_shape.setOrigin(size.x / 2.0f, size.y / 2.0f);
}

sf::Vector2f Hurtbox::getSize() const {
    return m_shape.getSize();
}

void Hurtbox::setOffset(const sf::Vector2f& offset) {
    m_offset = offset;
    
    // Actualizar la posición con el nuevo offset
    setPosition(m_position);
}

sf::Vector2f Hurtbox::getOffset() const {
    return m_offset;
}

void Hurtbox::setColor(const sf::Color& color) {
    m_shape.setFillColor(color);
}

void Hurtbox::setActive(bool active) {
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

bool Hurtbox::isActive() const {
    return m_active;
}

void Hurtbox::render(sf::RenderWindow& window) {
    // Solo renderizar si está activo
    if (m_active) {
        window.draw(m_shape);
    }
}

sf::FloatRect Hurtbox::getGlobalBounds() const {
    return m_shape.getGlobalBounds();
}