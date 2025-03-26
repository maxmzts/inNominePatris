// detectedArea.h
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

// Clase que representa un área detectable en el juego
class DetectedArea {
private:
    std::unique_ptr<sf::Shape> m_shape;

public:
    // Constructor que acepta cualquier forma derivada de sf::Shape
    template <typename T>
    DetectedArea(T shape);

    // Constructor específico para círculos (para mantener compatibilidad)
    DetectedArea(float radius);

    // Destructor virtual
    virtual ~DetectedArea() = default;

    // Métodos para acceder a la forma subyacente
    sf::Shape& getShape();
    const sf::Shape& getShape() const;

    // Delegación de métodos comunes de sf::Shape
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

    void setOrigin(const sf::Vector2f& position);
    void setOrigin(float x, float y);
    
    void setFillColor(const sf::Color& color);
    sf::Color getFillColor() const;
    
    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getLocalBounds() const;
    
    // Método para dibujar el área
    void draw(sf::RenderTarget& target) const;
};

// Implementación del constructor template (debe estar en el archivo .h)
template <typename T>
DetectedArea::DetectedArea(T shape) {
    static_assert(std::is_base_of<sf::Shape, T>::value, "T debe ser una clase derivada de sf::Shape");
    m_shape = std::make_unique<T>(std::move(shape));
}