#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Enemy.h"

// Declaraciones adelantadas para evitar dependencias circulares
// class Hitbox;
// class Hurtbox;
// class Character;
// class TileMap;

class EnemyBat: public Enemy {
private:

public:
    // Constructor y destructor
    EnemyBat(const std::string& name, float maxHealth, float movementSpeed, const sf::Vector2f& startPosition);

    // Animaciones
    void loadAnimations() override;
    void changeAnimation(EnemyState newState) override;
    
    // Funciones requeridas
    void takeDamage(float damage);
    void attack();
    void move(const sf::Vector2f& direction);
    
    void render(sf::RenderWindow& window);
    void update(float deltaTime, Character* player, const TileMap* tileMap) override;
};
