#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class Hitbox
 * @brief Define un área rectangular donde el enemigo puede realizar daño.
 * 
 * Esta clase representa una región que al colisionar con una Hurtbox de otro
 * objeto (como el jugador) causará daño. Internamente utiliza un sf::RectangleShape
 * para representación visual y cálculos de colisión.
 */
class Hitbox {
private:
    sf::RectangleShape m_shape;      // Forma rectangular que define el área de ataque
    sf::Vector2f m_offset;           // Desplazamiento relativo a la posición del dueño
    sf::Vector2f m_position;         // Posición actual en el mundo
    bool m_active;                   // Indica si el hitbox está activo (puede causar daño)
    
public:
    /**
     * @brief Constructor de la clase Hitbox
     * @param size Tamaño del rectángulo (ancho, alto)
     * @param offset Desplazamiento relativo a la posición del dueño
     * @param color Color para la representación visual (opcional)
     */
    Hitbox(const sf::Vector2f& size, const sf::Vector2f& offset, 
           const sf::Color& color = sf::Color(255, 0, 0, 128));
    
    /**
     * @brief Destructor de la clase Hitbox
     */
    ~Hitbox();
    
    /**
     * @brief Actualiza la posición del hitbox
     * @param position Nueva posición base
     */
    void setPosition(const sf::Vector2f& position);
    
    /**
     * @brief Obtiene la posición actual del hitbox
     * @return Posición actual en el mundo
     */
    sf::Vector2f getPosition() const;
    
    /**
     * @brief Cambia el tamaño del hitbox
     * @param size Nuevo tamaño (ancho, alto)
     */
    void setSize(const sf::Vector2f& size);
    
    /**
     * @brief Obtiene el tamaño actual del hitbox
     * @return Tamaño actual (ancho, alto)
     */
    sf::Vector2f getSize() const;
    
    /**
     * @brief Cambia el desplazamiento relativo
     * @param offset Nuevo desplazamiento
     */
    void setOffset(const sf::Vector2f& offset);
    
    /**
     * @brief Obtiene el desplazamiento actual
     * @return Desplazamiento actual
     */
    sf::Vector2f getOffset() const;
    
    /**
     * @brief Cambia el color de la representación visual
     * @param color Nuevo color
     */
    void setColor(const sf::Color& color);
    
    /**
     * @brief Activa o desactiva el hitbox
     * @param active Estado de activación
     */
    void setActive(bool active);
    
    /**
     * @brief Verifica si el hitbox está activo
     * @return true si está activo, false en caso contrario
     */
    bool isActive() const;
    
    /**
     * @brief Dibuja el hitbox en la ventana
     * @param window Ventana de renderizado
     */
    void render(sf::RenderWindow& window);
    
    /**
     * @brief Obtiene los límites globales del hitbox para cálculos de colisión
     * @return Rectángulo con las coordenadas globales
     */
    sf::FloatRect getGlobalBounds() const;
};