#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class Hurtbox
 * @brief Define un área rectangular donde el objeto puede recibir daño.
 * 
 * Esta clase representa una región que al ser colisionada por un Hitbox de otro
 * objeto (como un enemigo) recibirá daño. Internamente utiliza un sf::RectangleShape
 * para representación visual y cálculos de colisión.
 */
class Hurtbox {
private:
    sf::RectangleShape m_shape;      // Forma rectangular que define el área vulnerable
    sf::Vector2f m_offset;           // Desplazamiento relativo a la posición del dueño
    sf::Vector2f m_position;         // Posición actual en el mundo
    bool m_active;                   // Indica si el hurtbox está activo (puede recibir daño)
    
public:
    /**
     * @brief Constructor de la clase Hurtbox
     * @param size Tamaño del rectángulo (ancho, alto)
     * @param offset Desplazamiento relativo a la posición del dueño
     * @param color Color para la representación visual (opcional)
     */
    Hurtbox(const sf::Vector2f& size, const sf::Vector2f& offset, 
            const sf::Color& color = sf::Color(0, 0, 255, 128));
    
    /**
     * @brief Destructor de la clase Hurtbox
     */
    ~Hurtbox();
    
    /**
     * @brief Actualiza la posición del hurtbox
     * @param position Nueva posición base
     */
    void setPosition(const sf::Vector2f& position);
    
    /**
     * @brief Obtiene la posición actual del hurtbox
     * @return Posición actual en el mundo
     */
    sf::Vector2f getPosition() const;
    
    /**
     * @brief Cambia el tamaño del hurtbox
     * @param size Nuevo tamaño (ancho, alto)
     */
    void setSize(const sf::Vector2f& size);
    
    /**
     * @brief Obtiene el tamaño actual del hurtbox
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
     * @brief Activa o desactiva el hurtbox
     * @param active Estado de activación
     */
    void setActive(bool active);
    
    /**
     * @brief Verifica si el hurtbox está activo
     * @return true si está activo, false en caso contrario
     */
    bool isActive() const;
    
    /**
     * @brief Dibuja el hurtbox en la ventana
     * @param window Ventana de renderizado
     */
    void render(sf::RenderWindow& window);
    
    /**
     * @brief Obtiene los límites globales del hurtbox para cálculos de colisión
     * @return Rectángulo con las coordenadas globales
     */
    sf::FloatRect getGlobalBounds() const;
};