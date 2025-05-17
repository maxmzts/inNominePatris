#include "Boss1Minion.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>

Boss1Minion::Boss1Minion(const sf::Vector2f& startPosition, int DropkarmaPoints)
    : Enemy(
        "Minion", 
        50.f, 
        120.f, 
        startPosition, 
        "resources/enemies/boss1.png"
        ),
        attacked(false),
        targeted(false)
{
    sprite.setTextureRect(sf::IntRect(0,0, 220, 112));
    sprite.setOrigin(220/2, 112/2);
    sprite.setScale(0.5f,0.5f);
    hurtbox->setOffset({-4, 20.f});
    hitbox->setSize({30,30});
    loadAnimations();
    KarmaPoints = DropkarmaPoints;
}

/**
 * Carga las animaciones de este enemigo.
 */
void Boss1Minion::loadAnimations() {
    sf::Vector2i resolution = sf::Vector2i(220, 112);
    animator->addAnimation("idle",      10, sf::Vector2i(0, 0),                 resolution, true);
    animator->addAnimation("moving",    12, sf::Vector2i(0, resolution.y),      resolution, true);
    animator->addAnimation("attacking", 16, sf::Vector2i(0, resolution.y*2),    resolution, false);
    animator->addAnimation("dying",     17, sf::Vector2i(0, resolution.y*3),    resolution, false);
}

/**
 * Cambia la animación según el estado actual
 */
void Boss1Minion::changeAnimation(int newStateInt) {
    if (!isValidEnemyState(newStateInt)) return;
    EnemyState newState = static_cast<EnemyState>(newStateInt);
    stateTimer = 0.f;
    
    switch (newState) {
        case EnemyState::IDLE:
            animator->play("idle", 8.f); 
            break;
        case EnemyState::MOVING:
            animator->play("moving", 8.f); 
            break;
        case EnemyState::ATTACKING:
            animator->play("attacking", 16.f, false);
            break;
        case EnemyState::HURT:
            animator->play("idle", 0.f, false);
            break;
        case EnemyState::DYING:
            animator->play("dying", 8.f, false);
            break;
        case EnemyState::DEAD:
            break;
    }
}

void Boss1Minion::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    Enemy::takeDamage(damage, attackPosition);    
}

/**
 * Para atacar el enemigo pone activa la hitbox durante un corto periodo de tiempo.
 * El daño producido al jugador se manejará en el sistema de colisiones de InGame.
 * Que la hitbox vuelva a estar inactiva se gestiona en el update.
 */
void Boss1Minion::attack() {
    // Verificar si el ataque está en cooldown
    if (attackTimer > 0) {
        return;
    }
    
    // Cambiar al estado de ataque
    changeState(static_cast<int>(EnemyState::ATTACKING));
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

void Boss1Minion::calculateVelocity(const sf::Vector2f& direction) {
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
void Boss1Minion::render(sf::RenderWindow& window) {
    // Para debugging, podemos dibujar los hitboxes y hurtboxes
    hitbox->render(window);
    hurtbox->render(window);
    
    // Dibujar el sprite del enemigo
    sprite.draw(window);
}

// Primero, añade estos miembros a la clase Boss1Minion en el archivo de cabecera (Boss1Minion.h):

void Boss1Minion::update(float deltaTime, Character* player, const TileMap* tileMap) {
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

    sf::Vector2f toPlayer = player->getPosition() - position;
    float distanceSquared = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;
    
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
            // clava el objetivo en el jugador o su direccion
            if(stateTimer >= 0.5 && !targeted){
                if (distanceSquared <= 50.f * 50.f) {
                    attackPosition = player->getPosition();
                    targeted = true;
                }
                else {
                    float distance = std::sqrt(distanceSquared);
                    sf::Vector2f direction = toPlayer / distance;  // Normalizamos
                    attackPosition = position + direction * 50.f;
                    targeted = true;
                }
                
            }
            // la hitbox aparece
            if (stateTimer >= .68f && !attacked) {  // Duración de la animación de ataque
                hitbox->setPosition(attackPosition);
                hitbox->setActive(true);
                VFXManager::getInstance().addEffect("./resources/vfx/hit_on_ground.png",hitbox->getPosition(),{45 , 45},5,8.f);
                attacked = true;
            }
            if(stateTimer >= 0.75f){
                hitbox->setActive(false);
            }
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 1.f) {  // Duración de la animación de ataque
                changeState(static_cast<int>(EnemyState::IDLE));
                attacked = false;
                targeted = false;
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
                    "./resources/vfx/smoke_disappear.png",
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

void Boss1Minion::changeState(int newState){
    Enemy::changeState(newState);
};