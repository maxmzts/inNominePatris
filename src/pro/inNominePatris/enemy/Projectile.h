#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "SpriteFacade.h"
#include "Hitbox.h"
#include "AnimatedSprite.h"
#include "Hurtbox.h"

class Character;

class Projectile {
public:
    Projectile(sf::Vector2f position, sf::Vector2f velocity, float damage, float lifetime);
    ~Projectile();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool checkCollision(Character* player);
    bool isExpired() const;
    sf::Vector2f getPosition() const { return position; }
    float getDamage() const { return damage; }

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float damage;
    float lifetime;
    float elapsedTime;
    
    SpriteFacade sprite;
    AnimatedSprite* animatedSprite;
    Hitbox* hitbox;
};

#endif // PROJECTILE_H