#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "TileMap.h"
#include "Hitbox.h"    
#include "Hurtbox.h"
#include "Character.h"
#include "SpriteFacade.h"
#include "AnimatedSprite.h"

// Declaraciones adelantadas para evitar dependencias circulares
// class Hitbox;
// class Hurtbox;
// class Character;
// class TileMap;

class Enemy {
protected:
    std::string name;
    float maxHealth;
    float currentHealth;
    float movementSpeed;
    
    Hitbox* hitbox;         // Para hacer daño
    Hurtbox* hurtbox;       // Para recibir daño
    
    bool isInvincible;      // Estado de invencibilidad
    float invincibilityTimer; // Contador de invencibilidad
    float invincibilityDuration; // Duración máxima de invencibilidad
    
    sf::Vector2f position;
    sf::Vector2f velocity;
    
    // Visuales
    SpriteFacade sprite;
    AnimatedSprite* animator;
    bool animationsLoaded;
    bool facingRight;
    
    // Estados del enemigo
    enum class EnemyState {
        IDLE,
        MOVING,
        ATTACKING,
        HURT,
        DYING,
        DEAD
    };
    
    EnemyState currentState;
    float stateTimer;       // Tiempo en el estado actual
    
    // Atributos adicionales
    int attackDamage;
    float attackCooldown;
    float attackTimer;
    float detectionRadius;  // Radio para detectar al jugador
    
    // Path finding
    std::vector<sf::Vector2f> path; // Camino calculado hacia el jugador
    int currentPathIndex;   // Índice actual en el camino
    float pathUpdateTimer = 0.0f;  // Tiempo desde la última actualización del camino
    float pathUpdateInterval = 0.5f; // Intervalo para recalcular el camino (en segundos)

public:
    // Constructor y destructor
    Enemy(const std::string& name, float maxHealth, float movementSpeed, const sf::Vector2f& startPosition, const std::string& texturePath);
    ~Enemy();
    
    // Getters y setters básicos
    std::string getName() const { return name; }
    float getMaxHealth() const { return maxHealth; }
    float getCurrentHealth() const { return currentHealth; }
    float getMovementSpeed() const { return movementSpeed; }
    sf::Vector2f getPosition() const { return position; }
    
    void setPosition(const sf::Vector2f& position);
    void setTexture(const std::string& texturePath);
    virtual void loadAnimations() = 0;
    virtual void changeAnimation(EnemyState newState) = 0;
    
    // Funciones requeridas
    void takeDamage(float damage);
    void attack();
    void move(const sf::Vector2f& direction);
    
    // Algoritmo A*
    void findPathToPlayer(const Character* player, const TileMap* tileMap);
    
    void render(sf::RenderWindow& window);
    virtual void update(float deltaTime, Character* player, const TileMap* tileMap) = 0;
    
    // Funciones adicionales útiles
    void setInvincible(bool invincible);
    bool getisInvincible() const { return isInvincible; }
    bool isDead() const { if(currentState == EnemyState::DEAD) return true; else return false; }
    void updateHitboxes();
    int getAttackDamage() const { return attackDamage; }
    void setAttackDamage(float damage) { attackDamage = damage; }
    
    // Para la detección de colisiones
    Hitbox* getHitbox() const { return hitbox; }
    Hurtbox* getHurtbox() const { return hurtbox; }
    
    // Cambio de estados
    void changeState(EnemyState newState);
    EnemyState getCurrentState() const { return currentState; }
    // Actualizar el intervalo de recálculo del camino
    void setPathUpdateInterval(float interval) { pathUpdateInterval = interval; }
};
