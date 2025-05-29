#include "EnemyBat.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>

EnemyBat::EnemyBat(const sf::Vector2f& startPosition, int DropkarmaPoints)
    : Enemy(
        "Bat", 
        50.f, 
        120.f, 
        startPosition, 
        "./src/resources/enemies/Bat.png"
        )
{
    loadAnimations();
    KarmaPoints = DropkarmaPoints;
}

/**
 * Carga las animaciones de este enemigo.
 */
void EnemyBat::loadAnimations() {
    animator->addAnimation("idle", 4, sf::Vector2i(0, 0), sf::Vector2i(64, 64), true);
    animator->addAnimation("moving", 4, sf::Vector2i(0, 0), sf::Vector2i(64, 64), true);
    animator->addAnimation("attacking", 4, sf::Vector2i(0, 0), sf::Vector2i(64, 64), true);
    animator->addAnimation("dying", 7, sf::Vector2i(0, 64), sf::Vector2i(64, 64), false);
}

/**
 * Cambia la animación según el estado actual
 */
void EnemyBat::changeAnimation(int newStateInt) {
    if (!isValidEnemyState(newStateInt)) return;
    EnemyState newState = static_cast<EnemyState>(newStateInt);
    stateTimer = 0.f;
    
    switch (newState) {
        case EnemyState::IDLE:
            animator->play("idle", 8.f); // 8 FPS para idle
            break;
        case EnemyState::MOVING:
            animator->play("moving", 12.f); // 10 FPS para movimiento
            break;
        case EnemyState::ATTACKING:
            animator->play("attacking", 32.f); // Animación de ataque no en bucle
            break;
        case EnemyState::HURT:
            animator->play("idle", 0.f);
            break;
        case EnemyState::DYING:
            animator->play("dying", 8.f, false);
            break;
        case EnemyState::DEAD:
            break;
    }
}

void EnemyBat::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    Enemy::takeDamage(damage, attackPosition);    
}

/**
 * Para atacar el enemigo pone activa la hitbox durante un corto periodo de tiempo.
 * El daño producido al jugador se manejará en el sistema de colisiones de InGame.
 * Que la hitbox vuelva a estar inactiva se gestiona en el update.
 */
void EnemyBat::attack() {
    // Verificar si el ataque está en cooldown
    if (attackTimer > 0) {
        return;
    }
    
    // Cambiar al estado de ataque
    changeState(static_cast<int>(EnemyState::ATTACKING));
    VFXManager::getInstance().addEffect(
        "./src/resources/vfx/scratch.png",
        {hitbox->getPosition().x, hitbox->getPosition().y},          // posición de prueba
        {45 , 33},           // tamaño de frame
        6,                  // cantidad de frames
        16.f,                 // FPS
        !facingRight
    );
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

void EnemyBat::calculateVelocity(const sf::Vector2f& direction) {
    // Normalizar el vector de dirección si no es cero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        sf::Vector2f normalizedDir = direction / length;
        velocity = normalizedDir * movementSpeed;

        // Ajusta la dirección del sprite
        if (direction.x > 0 && !facingRight) {
            sprite.setScale(1.f, 1.f);
            facingRight = true;
        } else if (direction.x < 0 && facingRight) {
            sprite.setScale(-1.f, 1.f);
            facingRight = false;
        }
    } else {
        velocity = sf::Vector2f(0.f, 0.f);
    }
    
    // El movimiento real se aplica en update
}

/**
 * Funcion de dibujado de los enemigos.
 */
void EnemyBat::render(sf::RenderWindow& window) {
    // Para debugging, podemos dibujar los hitboxes y hurtboxes
    hitbox->render(window);
    hurtbox->render(window);
    
    // Dibujar el sprite del enemigo
    sprite.draw(window);
}

// Primero, añade estos miembros a la clase EnemyBat en el archivo de cabecera (EnemyBat.h):

void EnemyBat::update(float deltaTime, Character* player, const TileMap* tileMap) {
    // Actualizar los timers
    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            setInvincible(false);
        }
    }

    if (isInKnockback) {
        knockbackTimer -= deltaTime;
        if (knockbackTimer <= 0) {
            isInKnockback = false;
        }
    }
    
    if (attackTimer > 0) {
        attackTimer -= deltaTime;
    }
    
    // Actualizar el timer del estado actual
    stateTimer += deltaTime;
    
    // Actualizar el timer para recalcular el camino
    pathUpdateTimer += deltaTime;

    animator->update(deltaTime);

    // Si la vida llega a 0, cambiar al estado de muerte
    if (currentHealth <= 0 && currentState != EnemyState::DYING) {
        changeState(static_cast<int>(EnemyState::DYING));
    }
    
    // Lógica basada en el estado actual
    switch (currentState) {
        case EnemyState::IDLE:
            // En estado idle, buscar al jugador
            if (player) {
                // Recalcular el camino solo en intervalos específicos
                if (pathUpdateTimer >= pathUpdateInterval) {
                    pathUpdateTimer = 0;
                    findPathToPlayer(player, tileMap);
                }
                
                // Si comenzamos a movernos, cambiar al estado de movimiento
                if (velocity.x != 0 || velocity.y != 0) {
                    changeState(static_cast<int>(EnemyState::MOVING));
                }
            }
            break;
            
        case EnemyState::MOVING:
            // Recalcular el camino periódicamente mientras nos movemos
            if (player && pathUpdateTimer >= pathUpdateInterval) {
                pathUpdateTimer = 0;
                findPathToPlayer(player, tileMap);
            }

            // La velocidad de movimiento se calcula en el pathfinding
            move(tileMap, deltaTime);
            updateHitboxes();
            
            // Verificar si podemos atacar al jugador
            if (player) {
                sf::Vector2f playerPos = player->getPosition();
                sf::Vector2f direction = playerPos - position;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                
                // Si estamos lo suficientemente cerca y el ataque no está en cooldown
                if (distance < 50.f && attackTimer <= 0) {
                    attack();
                }
            }
            break;
            
        case EnemyState::ATTACKING:
            hitbox->setActive(true);                
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 0.5f) {  // Duración de la animación de ataque
                changeState(static_cast<int>(EnemyState::IDLE));
                hitbox->setActive(false);
            }
            break;
            
        case EnemyState::HURT:
            if(hitbox->isActive())
                hitbox->setActive(false);
            if(isInvincible)
                setInvincible(true);
            // Aplicar el movimiento de knockback
            knockback(deltaTime, tileMap);
            // La animación de daño duraría un tiempo fijo
            if (stateTimer >= 0.3f) {  // Duración de la animación de daño
                changeState(static_cast<int>(EnemyState::IDLE));
                setInvincible(false);
            }
            break;
            
        case EnemyState::DYING:
            hitbox->setActive(false);
            hurtbox->setActive(false);
            knockback(deltaTime, tileMap);
            if (stateTimer >= 3.f) {  // Duración de la animación de muerte
                VFXManager::getInstance().addEffect(
                    "./src/resources/vfx/smoke_disappear.png",
                    {sprite.getPosition().x, sprite.getPosition().y+15},
                    sf::Vector2i(64,32),
                    10,
                    12.f
                );
                changeState(static_cast<int>(EnemyState::DEAD));
            }
            break;
        case EnemyState::DEAD:
            break;
    }
}

void EnemyBat::changeState(int newState){
    Enemy::changeState(newState);
};