#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "TileMap.h" // clase TileMap para detección de colisiones
#include "GameEngine.h" // clase GameEngine para la ventana y eventos

class Character {
public:
    Character(const std::string& textureFile);

    void handleInput(const sf::Event& event);
    void update(const TileMap& tilemap); // update recibe el mapa para verificar colisiones
    void draw(GameEngine& engine);

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    
    float speed;
    float acceleration;
    float deceleration;

    bool movingRight = false;
    bool movingLeft = false;
    bool movingUp = false;
    bool movingDown = false;
};

#endif // CHARACTER_H
