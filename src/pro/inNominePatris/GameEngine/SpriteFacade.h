#ifndef SPRITEFACADE_H
#define SPRITEFACADE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class SpriteFacade {
private:
    sf::Sprite sprite;       // Sprite interno
    sf::Texture texture;     // Textura asociada al sprite

public:
    // Constructor vacío
    SpriteFacade();

    // Constructor que carga una textura desde un archivo
    SpriteFacade(const std::string& texturePath);

    // Cargar textura desde un archivo
    bool loadTexture(const std::string& texturePath);

    void loadTexture(const sf::Texture& texture);

    // Establecer la posición del sprite
    void setPosition(float x, float y);

    void setRotation(float angle);

    void setTextureRect(sf::IntRect rect);

    // Obtener la posición del sprite
    sf::Vector2f getPosition() const;

    // Establecer el origen del sprite
    void setOrigin(float x, float y);

    // Escalar el sprite
    void setScale(float scaleX, float scaleY);

    // Rotar el sprite
    void rotate(float angle);

    // Dibujar el sprite en una ventana
    void draw(sf::RenderWindow& window) const;

    // Obtener el tamaño global del sprite
    sf::FloatRect getGlobalBounds() const;

    // Obtener el tamaño local del sprite
    sf::FloatRect getLocalBounds() const;

    // Establecer el color del sprite
    void setColor(const sf::Color& color);

    const sf::Sprite& getSprite() const;

    void move(sf::Vector2f offset);
};

#endif // SPRITEFACADE_H