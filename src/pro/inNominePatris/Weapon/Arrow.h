#ifndef ARROW_H
#define ARROW_H

#include <SFML/Graphics.hpp>

class Arrow {
public:
    Arrow(const sf::Vector2f& position, const sf::Vector2f& direction, float speed);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    const sf::FloatRect getBounds() const;

    void markforRemoval();
    bool isMarkedForRemoval() const;
    sf::Vector2f getPosition() { return sprite.getPosition(); } 

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool markedForRemoval = false;

    static sf::Texture arrowTexture; // Textura de la flecha (cargada una vez y compartida entre todas las flechas)
};

#endif // !ARROW_H