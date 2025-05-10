#include "EnemyNecromancer.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>
#include <EnemyManager.h>
#include "EnemyBat.h"

EnemyNecromancer::EnemyNecromancer(const sf::Vector2f& startPosition, int DropKarmaPoints)
    : Enemy(
        "Necromancer", 
        150.f, 
        80.f, 
        startPosition, 
        "resources/enemies/necromancer.png"),
        spawnTimer(5.f),
        attacked(false), //spawnea nuevos enemigos cada 5 segundos
        currentNecromancerState(NecromancerState::IDLE)
{
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 96));
    loadAnimations();
    attackCooldown = 10.f;
    KarmaPoints = DropKarmaPoints; 
}

/**
 * Carga las animaciones de este enemigo.
 */
void EnemyNecromancer::loadAnimations() {
    animator->addAnimation("idle", 8, sf::Vector2i(0, 0), sf::Vector2i(64, 96), true);
    animator->addAnimation("moving", 8, sf::Vector2i(0, 96), sf::Vector2i(64, 96), true);
    // spawns a bat
    animator->addAnimation("spawning", 13, sf::Vector2i(0, 96*2), sf::Vector2i(64, 96), false);
    // heals himself and the rest of enemies
    animator->addAnimation("healing", 13, sf::Vector2i(0, 96*3), sf::Vector2i(64, 96), false);
    animator->addAnimation("attacking", 17, sf::Vector2i(0, 96*4), sf::Vector2i(64, 96), false);
    animator->addAnimation("hurt", 5, sf::Vector2i(0, 96*5), sf::Vector2i(64, 96), false);
    animator->addAnimation("dying", 12, sf::Vector2i(0, 96*6), sf::Vector2i(64, 96), false);
}

/**
 * Cambia la animación según el estado actual
 */
void EnemyNecromancer::changeAnimation(int newStateInt) {
    if (!isValidNecromancerState(newStateInt)) return;
    NecromancerState newState = static_cast<NecromancerState>(newStateInt);
    stateTimer = 0.f;
    
    switch (newState) {
        case NecromancerState::IDLE:
            animator->play("idle", 8.f); 
            break;
        case NecromancerState::MOVING:
            animator->play("moving", 8.f); 
            break;
        case NecromancerState::ATTACKING:
            animator->play("attacking", 8.f, false);
            break;
        case NecromancerState::HEALING:
            animator->play("healing", 8.f, false);
            break;
        case NecromancerState::SPAWNING:
            animator->play("spawning", 8.f, false);
            break;
        case NecromancerState::HURT:
            animator->play("hurt", 8.f, false);
            break;
        case NecromancerState::DYING:
            animator->play("dying", 8.f, false);
            break;
        case NecromancerState::DEAD:
            break;
    }
}

void EnemyNecromancer::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    // Si está invencible, ignorar el daño
    if (isInvincible || currentNecromancerState == NecromancerState::DYING || currentNecromancerState == NecromancerState::DEAD)
        return;
    
    currentHealth -= damage;
    
    // Asegurar que la vida no baje de 0
    if (currentHealth < 0) {
        currentHealth = 0;
    }
    
    // Cambiar al estado de herido, pero no se inmuta si te está atacando o está spawneando murcielagos
    if(currentNecromancerState != NecromancerState::ATTACKING && currentNecromancerState != NecromancerState::SPAWNING)
        changeState(static_cast<int>(NecromancerState::HURT));
    
    // Activar invencibilidad
    setInvincible(true);

    sf::Vector2f attackDirection = attackPosition-position;
    setupKnockback(attackDirection, 100.0f);
    
    // Si la vida llega a 0, cambiar al estado de muerte
    if (currentHealth <= 0) {
        changeState(static_cast<int>(NecromancerState::DYING));
    }
}

/**
 * Para atacar el enemigo pone activa la hitbox durante un corto periodo de tiempo.
 * El daño producido al jugador se manejará en el sistema de colisiones de InGame.
 * Que la hitbox vuelva a estar inactiva se gestiona en el update.
 */
void EnemyNecromancer::attack() {
    // Verificar si el ataque está en cooldown
    if (attackTimer > 0) {
        return;
    }
    
    // Cambiar al estado de ataque
    changeState(static_cast<int>(NecromancerState::ATTACKING));
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

void EnemyNecromancer::move(const sf::Vector2f& direction) {
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
void EnemyNecromancer::render(sf::RenderWindow& window) {
    // Para debugging, podemos dibujar los hitboxes y hurtboxes
    hitbox->render(window);
    hurtbox->render(window);
    
    // Dibujar el sprite del enemigo
    sprite.draw(window);
}

// Primero, añade estos miembros a la clase EnemyNecromancer en el archivo de cabecera (EnemyNecromancer.h):

void EnemyNecromancer::update(float deltaTime, Character* player, const TileMap* tileMap) {
    // Actualizar los timers
    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            setInvincible(false);
        }
    }

    if (spawnTimer > 0) {
        spawnTimer -= deltaTime;
    }
    
    if (attackTimer > 0) {
        attackTimer -= deltaTime;
    }
    
    // Actualizar el timer del estado actual
    stateTimer += deltaTime;
    
    // Actualizar el timer para recalcular el camino
    pathUpdateTimer += deltaTime;

    animator->update(deltaTime);
    
    // Lógica basada en el estado actual
    switch (currentNecromancerState) {
        case NecromancerState::IDLE:
            // En estado idle, buscar al jugador
            if (player) {
                // Recalcular el camino solo en intervalos específicos
                if (pathUpdateTimer >= pathUpdateInterval) {
                    pathUpdateTimer = 0;
                    findPathToPlayer(player, tileMap);
                }
                
                // Si comenzamos a movernos, cambiar al estado de movimiento
                if (velocity.x != 0 || velocity.y != 0) {
                    changeState(static_cast<int>(NecromancerState::MOVING));
                }
            }
            break;
            
        case NecromancerState::MOVING:
            // Aplicar velocidad al movimiento
            position += velocity * deltaTime;
            sprite.setPosition(position.x, position.y);
            updateHitboxes();
            
            // Recalcular el camino periódicamente mientras nos movemos
            if (player && pathUpdateTimer >= pathUpdateInterval) {
                pathUpdateTimer = 0;
                findPathToPlayer(player, tileMap);
            }
            
            // Verificar si podemos atacar al jugador
            if (player && attackTimer <= 0) {
                attackPosition = player->getPosition();
                VFXManager::getInstance().addEffect(
                    "./resources/vfx/anticipation.png",
                    attackPosition,          // posición de prueba
                    {45 , 45},           // tamaño de frame
                    12,                  // cantidad de frames
                    12.f
                );
                attack();
            } // spawnear murcielago
            else if (player && spawnTimer <= 0) {
                changeState(static_cast<int>(NecromancerState::SPAWNING));
            }
            
            break;
            
        case NecromancerState::ATTACKING:
            if (stateTimer >= 1.5f && !attacked) {  // Duración de la animación de ataque
                hitbox->setPosition(attackPosition);
                hitbox->setActive(true);
                VFXManager::getInstance().addEffect(
                    "./resources/vfx/explosion64x64.png",
                    attackPosition,  
                    {64 , 64},           
                    10,                 
                    16.f
                );
                attacked = true;
            }
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 2.f) {  // Duración de la animación de ataque
                changeState(static_cast<int>(NecromancerState::IDLE));
                hitbox->setActive(false);
                attacked = false;
            }
            break;
        case NecromancerState::HURT:
            if(hitbox->isActive())
                hitbox->setActive(false);
            setInvincible(true);
            // La animación de daño duraría un tiempo fijo
            if (stateTimer >= 0.3f) {  // Duración de la animación de daño
                changeState(static_cast<int>(NecromancerState::IDLE));
                setInvincible(false);
            }
            break;
        case NecromancerState::SPAWNING:
            // La animación de spawn
            if (stateTimer >= 1.f) {  // Duración de la animación de daño
                // spawnea un enemigo
                spawn();
                changeState(static_cast<int>(NecromancerState::IDLE));
                spawnTimer = 5.f;
            }
            break;
        case NecromancerState::HEALING:
            // La animación de daño duraría un tiempo fijo
            
            break;
            
        case NecromancerState::DYING:
            hitbox->setActive(false);
            hurtbox->setActive(false);
            if (stateTimer >= 1.f) {  // Duración de la animación de muerte
                changeState(static_cast<int>(NecromancerState::DEAD));
            }
            break;
        case NecromancerState::DEAD:
            break;
    }
}

void EnemyNecromancer::changeState(int newStateInt) {
    if (!isValidNecromancerState(newStateInt)) return;
    NecromancerState newState = static_cast<NecromancerState>(newStateInt);

    // Si estamos cambiando a un nuevo estado, reiniciar el timer
    if (currentNecromancerState != newState) {
        currentNecromancerState = newState;
        stateTimer = 0.0f;
        changeAnimation(newStateInt);
        // Acciones específicas al cambiar de estado podrían ir aquí
        // Por ejemplo, cambiar la animación según el estado
    }
}

bool EnemyNecromancer::isValidNecromancerState(int state){
    return state >= static_cast<int>(NecromancerState::IDLE) &&
           state <= static_cast<int>(NecromancerState::DEAD);
}

bool EnemyNecromancer::isDead() const{
    if(currentNecromancerState == NecromancerState::DEAD) 
        return true; 
    else 
        return false; 
}

void EnemyNecromancer::spawn(){
    EnemyManager::getInstance()->addEnemy(
        std::make_shared<EnemyBat>(position, 0)
    );
    // anyadir efecto en el futuro
}