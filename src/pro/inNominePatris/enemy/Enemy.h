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
private:
    std::string m_name;
    float m_maxHealth;
    float m_currentHealth;
    float m_movementSpeed;
    
    Hitbox* m_hitbox;         // Para hacer daño
    Hurtbox* m_hurtbox;       // Para recibir daño
    
    bool m_isInvincible;      // Estado de invencibilidad
    float m_invincibilityTimer; // Contador de invencibilidad
    float m_invincibilityDuration; // Duración máxima de invencibilidad
    
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    
    // Visuales
    SpriteFacade m_sprite;
    AnimatedSprite* m_animator;
    bool m_animationsLoaded;
    bool m_facingRight;
    
    // Estados del enemigo
    enum class EnemyState {
        IDLE,
        MOVING,
        ATTACKING,
        HURT,
        DYING
    };
    
    EnemyState m_currentState;
    float m_stateTimer;       // Tiempo en el estado actual
    
    // Atributos adicionales
    float m_attackDamage;
    float m_attackCooldown;
    float m_attackTimer;
    float m_detectionRadius;  // Radio para detectar al jugador
    
    // Path finding
    std::vector<sf::Vector2f> m_path; // Camino calculado hacia el jugador
    int m_currentPathIndex;   // Índice actual en el camino
    float m_pathUpdateTimer = 0.0f;  // Tiempo desde la última actualización del camino
    float m_pathUpdateInterval = 0.5f; // Intervalo para recalcular el camino (en segundos)

public:
    // Constructor y destructor
    Enemy(const std::string& name, float maxHealth, float movementSpeed, const sf::Vector2f& startPosition);
    ~Enemy();
    
    // Getters y setters básicos
    std::string getName() const { return m_name; }
    float getMaxHealth() const { return m_maxHealth; }
    float getCurrentHealth() const { return m_currentHealth; }
    float getMovementSpeed() const { return m_movementSpeed; }
    sf::Vector2f getPosition() const { return m_position; }
    
    void setPosition(const sf::Vector2f& position);
    void setTexture(const std::string& texturePath);
    void loadAnimations();
    void changeAnimation(EnemyState newState);
    
    // Funciones requeridas
    void takeDamage(float damage);
    void attack();
    void move(const sf::Vector2f& direction);
    
    // Algoritmo A*
    void findPathToPlayer(const Character* player, const TileMap* tileMap);
    
    void render(sf::RenderWindow& window);
    void update(float deltaTime, Character* player, const TileMap* tileMap);
    
    // Funciones adicionales útiles
    void setInvincible(bool invincible);
    bool isInvincible() const { return m_isInvincible; }
    bool isAlive() const { return m_currentHealth > 0; }
    void updateHitboxes();
    float getAttackDamage() const { return m_attackDamage; }
    void setAttackDamage(float damage) { m_attackDamage = damage; }
    
    // Para la detección de colisiones
    Hitbox* getHitbox() const { return m_hitbox; }
    Hurtbox* getHurtbox() const { return m_hurtbox; }
    
    // Cambio de estados
    void changeState(EnemyState newState);
    EnemyState getCurrentState() const { return m_currentState; }
    // Actualizar el intervalo de recálculo del camino
    void setPathUpdateInterval(float interval) { m_pathUpdateInterval = interval; }
};
