#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite {  // Ahora hereda de sf::Sprite, como Character
private:
    sf::Sprite sprite;
    sf::Texture texture;
    int health;

public:
    Enemy(float x, float y);
    void setTexture(const sf::Texture& tex);
    void setTextureRect(int left, int top, int width, int height);
    void takeDamage(int amount);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window);
};

#endif // ENEMY_H
