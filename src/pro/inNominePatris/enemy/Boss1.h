#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Enemy.h"

class Boss1: public Enemy {
private:
    enum class Boss1State : int {
        IDLE,
        MOVING,
        ATTACKING,
        HURT,
        SPAWNING,
        DYING,
        DEAD
    };

    // variables auxiliares para el spawn
    float spawnTimer = 10.f;
    int spawnedEnemiesCount = 0; 
    int enemiesToSpawn = 0;
    float spawnIntervalTimer = 0.f;
    int r = 0, g = 0, b = 0, t = 1;

    bool attacked, targeted;
    sf::Vector2f attackPosition;
    Boss1State currentBoss1State;

public:
    // Constructor y destructor
    Boss1(const sf::Vector2f& startPosition, int DropKarmaPoints);

    // Animaciones
    void loadAnimations() override;
    void changeAnimation(int newState) override;
    
    // Funciones requeridas
    void takeDamage(float damage, const sf::Vector2f& attackPosition) override;
    void attack() override;
    void calculateVelocity(const sf::Vector2f& direction);
    
    void render(sf::RenderWindow& window);
    void update(float deltaTime, Character* player, const TileMap* tileMap) override;
    void updateTimers(float deltaTime);

    void move(const TileMap* tileMap, float deltaTime);
    void changeState(int newState) override;
    bool isValidBoss1State(int state);
    bool isDead() const override;

    void spawn();
};
