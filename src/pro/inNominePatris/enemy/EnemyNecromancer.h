#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Enemy.h"

class EnemyNecromancer: public Enemy {
private:
    enum class NecromancerState : int {
        IDLE,
        MOVING,
        ATTACKING,
        HURT,
        SPAWNING,
        HEALING,
        DYING,
        DEAD
    };

    float spawnTimer;
    bool attacked;
    sf::Vector2f attackPosition;
    NecromancerState currentNecromancerState;

public:
    // Constructor y destructor
    EnemyNecromancer(const sf::Vector2f& startPosition);

    // Animaciones
    void loadAnimations() override;
    void changeAnimation(int newState) override;
    
    // Funciones requeridas
    void takeDamage(float damage, const sf::Vector2f& attackPosition) override;
    void attack() override;
    void move(const sf::Vector2f& direction);
    
    void render(sf::RenderWindow& window);
    void update(float deltaTime, Character* player, const TileMap* tileMap) override;

    void changeState(int newState) override;
    bool isValidNecromancerState(int state);

    void spawn();
};
