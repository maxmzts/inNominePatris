#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "Enemy.h"

class Character{
private:
    sf::Sprite sprite;
    sf::Texture texture; // Textura del personaje
    sf::Vector2f velocity;
    sf::Vector2f direction;
    Weapon* equippedWeapon;
    float speed;
    bool isDashing;
    sf::Clock dashTimer;
    float dashSpeed;
    float dashDuration;

public:
    Character();
    void setTexture(const sf::Texture& tex); // Método para asignar textura
    void setTextureRect(int left, int top, int width, int height); // Método para cambiar el rectángulo de la textura
    void equipWeapon(Weapon* newWeapon);
    void setScale(float x, float y);
    void setOrigin(float x, float y);
    void setPosition(float x, float y);
    void setDirection(float x, float y);
    sf::Vector2f getScale() const;
    void move(float x, float y);
    void attack(std::vector<Enemy>& enemies);
    void useAbility();
    void startDash(float speed, float duration);
    void increaseDashSpeed(float speed);
    void increaseSpeed(float speed);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
};

#endif // CHARACTER_H
