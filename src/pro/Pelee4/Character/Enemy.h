#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite sprite;
    int health;

public:
    Enemy(float x, float y);
    void takeDamage(int amount);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window);
};

#endif // ENEMY_H
