#include "Boss1.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>
#include <EnemyManager.h>
#include "EnemyBat.h"

Boss1::Boss1(const sf::Vector2f& startPosition, int DropKarmaPoints)
    : Enemy(
        "Boss1", 
        150.f, 
        80.f, 
        startPosition, 
        "resources/enemies/boss1.png"),
        spawnTimer(5.f),
        attacked(false), //spawnea nuevos enemigos cada 5 segundos
        currentBoss1State(Boss1State::IDLE)
{
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 96));
    loadAnimations();
    attackCooldown = 4.f;
    KarmaPoints = DropKarmaPoints; 
}

/**
 * Carga las animaciones de este enemigo.
 */
void Boss1::loadAnimations() {
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
void Boss1::changeAnimation(int newStateInt) {
    if (!isValidBoss1State(newStateInt)) return;
    Boss1State newState = static_cast<Boss1State>(newStateInt);
    stateTimer = 0.f;
    
    switch (newState) {
        case Boss1State::IDLE:
            animator->play("idle", 8.f); 
            break;
        case Boss1State::MOVING:
            animator->play("moving", 8.f); 
            break;
        case Boss1State::ATTACKING:
            animator->play("attacking", 8.f, false);
            break;
        case Boss1State::HEALING:
            animator->play("healing", 8.f, false);
            break;
        case Boss1State::SPAWNING:
            animator->play("spawning", 8.f, false);
            break;
        case Boss1State::HURT:
            animator->play("hurt", 8.f, false);
            break;
        case Boss1State::DYING:
            animator->play("dying", 8.f, false);
            break;
        case Boss1State::DEAD:
            break;
    }
}

/**
 * Resta vida al enemigo, prepara la invencibilidad, el knockback y además cambia de estado si así lo conviene
 */
void Boss1::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    // Si está invencible, ignorar el daño
    if (isInvincible || currentBoss1State == Boss1State::DYING || currentBoss1State == Boss1State::DEAD)
        return;
    
    currentHealth -= damage;
    
    // Asegurar que la vida no baje de 0
    if (currentHealth < 0) {
        currentHealth = 0;
    }
    
    // Cambiar al estado de herido, pero no se inmuta si te está atacando o está spawneando murcielagos
    if(currentBoss1State != Boss1State::ATTACKING && currentBoss1State != Boss1State::SPAWNING)
        changeState(static_cast<int>(Boss1State::HURT));
    
    // Activar invencibilidad
    setInvincible(true);

    sf::Vector2f attackDirection = attackPosition-position;
    setupKnockback(attackDirection, 100.0f);
    
    // Si la vida llega a 0, cambiar al estado de muerte
    if (currentHealth <= 0) {
        changeState(static_cast<int>(Boss1State::DYING));
    }
}

/**
 * Cambia al estado de ataque si el ataque no está en cooldown.
 * El daño producido al jugador se manejará en el sistema de colisiones de InGame.
 * Que la hitbox vuelva a estar inactiva se gestiona en el update.
 */
void Boss1::attack() {
    // Verificar si el ataque está en cooldown
    if (attackTimer > 0) {
        return;
    }
    
    // Cambiar al estado de ataque
    changeState(static_cast<int>(Boss1State::ATTACKING));
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

/**
 * Funcion de dibujado de los enemigos.
 */
void Boss1::render(sf::RenderWindow& window) {
    // DEBUG
    hitbox->render(window);
    hurtbox->render(window);
    
    // Dibujar el sprite del enemigo
    sprite.draw(window);
}


void Boss1::update(float deltaTime, Character* player, const TileMap* tileMap) {
    updateTimers(deltaTime);
    animator->update(deltaTime);
    
    // Lógica basada en el estado actual
    switch (currentBoss1State) {

        case Boss1State::IDLE:
            if (player) {
                // Recalcular el camino solo en intervalos específicos
                if (pathUpdateTimer >= pathUpdateInterval) {
                    pathUpdateTimer = 0;
                    findPathToPlayer(player, tileMap);
                }
                if (velocity.x != 0 || velocity.y != 0)
                    changeState(static_cast<int>(Boss1State::MOVING));
            }
            break;
            
        case Boss1State::MOVING:
            // Recalcular el camino periódicamente mientras nos movemos
            if (player && pathUpdateTimer >= pathUpdateInterval) {
                pathUpdateTimer = 0;
                findPathToPlayer(player, tileMap);
            }
            // Aplicar velocidad al movimiento calculada en el pathfinding
            move(tileMap, deltaTime);
            updateHitboxes();
            
            // Verificar si podemos atacar al jugador
            if (player && attackTimer <= 0) {
                attackPosition = player->getPosition();
                VFXManager::getInstance().addEffect("./resources/vfx/anticipation.png",attackPosition,{45 , 45},12,12.f);
                attack();
            } // spawnear murcielago
            else if (player && spawnTimer <= 0)
                changeState(static_cast<int>(Boss1State::SPAWNING));
            
            break;
            
        case Boss1State::ATTACKING:
            // ataca después de 1.5s que dura la anticipación
            if (stateTimer >= 1.5f && !attacked) {  // Duración de la animación de ataque
                hitbox->setPosition(attackPosition);
                hitbox->setActive(true);
                VFXManager::getInstance().addEffect("./resources/vfx/explosion64x64.png",attackPosition,{64 , 64},10,16.f);
                attacked = true;
            }
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 2.f) {  // Duración de la animación de ataque
                changeState(static_cast<int>(Boss1State::IDLE));
                hitbox->setActive(false);
                attacked = false;
            }
            break;

        case Boss1State::HURT:
            if(hitbox->isActive())
                hitbox->setActive(false);
            setInvincible(true);
            // La animación de daño duraría un tiempo fijo
            if (stateTimer >= 0.3f) {  // Duración de la animación de daño
                changeState(static_cast<int>(Boss1State::IDLE));
                setInvincible(false);
            }
            break;

        case Boss1State::SPAWNING:
            // La animación de spawn
            if (stateTimer >= 1.f) {  // Duración de la animación de daño
                // spawnea un enemigo
                spawn();
                changeState(static_cast<int>(Boss1State::IDLE));
                spawnTimer = 5.f;
            }
            break;

        case Boss1State::HEALING:
            // La animación de daño duraría un tiempo fijo
            
            break;
            
        case Boss1State::DYING:
            hitbox->setActive(false);
            hurtbox->setActive(false);
            if (stateTimer >= 1.f) {  // Duración de la animación de muerte
                changeState(static_cast<int>(Boss1State::DEAD));
            }
            break;
        case Boss1State::DEAD:
            break;
    }
    // DEBUG
    // std::cout << "Boss1,   x: " << position.x << " y: " << position.y << std::endl;
}

void Boss1::updateTimers(float deltaTime){
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
    
    stateTimer += deltaTime;
    
    pathUpdateTimer += deltaTime;
}

void Boss1::changeState(int newStateInt) {
    if (!isValidBoss1State(newStateInt)) return;
    Boss1State newState = static_cast<Boss1State>(newStateInt);

    // Si estamos cambiando a un nuevo estado, reiniciar el timer
    if (currentBoss1State != newState) {
        currentBoss1State = newState;
        stateTimer = 0.0f;
        changeAnimation(newStateInt);
        // Acciones específicas al cambiar de estado podrían ir aquí
        // Por ejemplo, cambiar la animación según el estado
    }
}

bool Boss1::isValidBoss1State(int state){
    return state >= static_cast<int>(Boss1State::IDLE) &&
           state <= static_cast<int>(Boss1State::DEAD);
}

bool Boss1::isDead() const{
    return (currentBoss1State == Boss1State::DEAD);
}

void Boss1::spawn(){
    EnemyManager::getInstance()->addEnemy(
        std::make_shared<EnemyBat>((position/16.f), 0)
    );
    // anyadir efecto en el futuro
}