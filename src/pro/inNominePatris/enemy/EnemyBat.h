#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Enemy.h"

class EnemyBat: public Enemy {
public:
    // Constructor y destructor
    EnemyBat(const sf::Vector2f& startPosition, int karmaPoints);

    // Animaciones
    void loadAnimations() override;
    void changeAnimation(int newStateInt) override;
    
    // Funciones requeridas
    void takeDamage(float damage, const sf::Vector2f& attackPosition) override;
    void attack() override;
    void calculateVelocity(const sf::Vector2f& direction);
    
    void render(sf::RenderWindow& window);
    void update(float deltaTime, Character* player, const TileMap* tileMap) override;

    virtual void changeState(int newState) override;
};
