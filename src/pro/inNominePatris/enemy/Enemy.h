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

    int KarmaPoints; // Puntos de karma que suelta al morir
    
    sf::Vector2f position;
    sf::Vector2f velocity;
    
    // Visuales
    SpriteFacade sprite;
    AnimatedSprite* animator;
    bool animationsLoaded;
    bool facingRight;
    
    // Estados del enemigo
    enum class EnemyState : int {
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

    // Knockback
    sf::Vector2f knockbackDirection;
    float knockbackForce;
    float knockbackDuration;
    float knockbackTimer;
    bool isInKnockback;

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
    virtual void changeAnimation(int newStateInt) = 0;
    
    // Funciones requeridas
    virtual void takeDamage(float damage, const sf::Vector2f& attackPosition) = 0;
    virtual void attack() = 0;
    void move(const sf::Vector2f& direction);
    void setupKnockback(const sf::Vector2f& attackDirection, float force);
    void knockback(float deltaTime, const TileMap* tileMap);
    
    // Algoritmo A*
    void findPathToPlayer(const Character* player, const TileMap* tileMap);
    
    void render(sf::RenderWindow& window);
    virtual void update(float deltaTime, Character* player, const TileMap* tileMap) = 0;
    
    // Funciones adicionales útiles
    void setInvincible(bool invincible);
    bool getisInvincible() const { return isInvincible; }
    virtual bool isDead() const;
    void updateHitboxes();
    float getAttackDamage() const { return attackDamage; }
    void setAttackDamage(float damage) { attackDamage = damage; }
    
    // Para la detección de colisiones
    Hitbox* getHitbox() const { return hitbox; }
    Hurtbox* getHurtbox() const { return hurtbox; }

    //Obtener los puntos de karma
    int getKarmaPoints() const { return KarmaPoints; }
    
    // Cambio de estados
    virtual void changeState(int newState) = 0;
    int getCurrentState() const { return static_cast<int>(currentState); }
    // Actualizar el intervalo de recálculo del camino
    void setPathUpdateInterval(float interval) { pathUpdateInterval = interval; }
    bool isValidEnemyState(int state);
};
