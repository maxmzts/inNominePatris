#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Enemy.h"

class Boss3: public Enemy {
private:
    enum class Boss3State : int {
        IDLE,
        MOVING,
        ATTACKING,
        HURT,
        JUMPING,
        DYING,
        DEAD
    };

    float jumpTimer;

    bool attacked, targeted;
    sf::Vector2f jumpPosition;
    float previousMovementSpeed;
    sf::Vector2f attackPosition;
    Boss3State currentBoss3State;
public:
    // Constructor y destructor
    Boss3(const sf::Vector2f& startPosition, int karmaPoints);

    // Animaciones
    void loadAnimations() override;
    void changeAnimation(int newStateInt) override;
    
    // Funciones requeridas
    void takeDamage(float damage, const sf::Vector2f& attackPosition) override;
    void attack() override;
    
    void render(sf::RenderWindow& window);
    void update(float deltaTime, Character* player, const TileMap* tileMap) override;
    void updateTimers(float deltaTime);

    void move(const TileMap* tileMap, float deltaTime);
    void changeState(int newState) override;
    bool isValidBoss3State(int state);
    bool isDead() const override;
};
