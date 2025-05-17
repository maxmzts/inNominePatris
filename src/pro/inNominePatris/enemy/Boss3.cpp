#include "Boss3.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>

#define WIDTH 256
#define HEIGHT 160

Boss3::Boss3(const sf::Vector2f& startPosition, int DropkarmaPoints)
    : Enemy(
        "Bat", 
        100.f, 
        50.f, 
        startPosition, 
        "resources/enemies/boss3.png"),
        jumpTimer(5.f),
        attacked(false),
        targeted(false), 
        previousMovementSpeed(movementSpeed),
        currentBoss3State(Boss3State::IDLE)
{
    sprite.setTextureRect(sf::IntRect(0,0, WIDTH, HEIGHT));
    sprite.setOrigin(120, 125);
    sprite.setScale(1.5f,1.5f);
    hurtbox->setSize({80, 45.f});
    // hurtbox->setOffset({-8, 60.f});
    hitbox->setSize({45,45});
    loadAnimations();
    KarmaPoints = DropkarmaPoints;
}

/**
 * Carga las animaciones de este enemigo.
 */
void Boss3::loadAnimations() {
    sf::Vector2i resolution = sf::Vector2i(WIDTH, HEIGHT);
    animator->addAnimation("idle",      9,  sf::Vector2i(0, 0),                 resolution, true);
    animator->addAnimation("moving",    10, sf::Vector2i(0, resolution.y),      resolution, true);
    animator->addAnimation("attacking", 12, sf::Vector2i(0, resolution.y*2),    resolution, false);
    animator->addAnimation("jumping",   15, sf::Vector2i(0, resolution.y*3),    resolution, false);
    animator->addAnimation("hurt",      8,  sf::Vector2i(0, resolution.y*4),    resolution, false);
    animator->addAnimation("dying",     22, sf::Vector2i(0, resolution.y*5),    resolution, false);
}

/**
 * Cambia la animación según el estado actual
 */
void Boss3::changeAnimation(int newStateInt) {
    if (!isValidBoss3State(newStateInt)) return;
    Boss3State newState = static_cast<Boss3State>(newStateInt);
    stateTimer = 0.f;
    
    switch (newState) {
        case Boss3State::IDLE:
            animator->play("idle", 10.f); 
            break;
        case Boss3State::MOVING:
            animator->play("moving", 10.f); 
            break;
        case Boss3State::ATTACKING:
            animator->play("attacking", 10.f, false);
            break;
        case Boss3State::JUMPING:
            animator->play("jumping", 10.f);
            break;
        case Boss3State::HURT:
            animator->play("hurt", 10.f, false);
            break;
        case Boss3State::DYING:
            animator->play("dying", 10.f, false);
            break;
        case Boss3State::DEAD:
            break;
    }
}

/**
 * Resta vida al enemigo, prepara la invencibilidad, el knockback y además cambia de estado si así lo conviene
 */
void Boss3::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    // Si está invencible, ignorar el daño
    if (isInvincible || currentBoss3State == Boss3State::DYING || currentBoss3State == Boss3State::DEAD)
        return;
    
    currentHealth -= damage;
    
    // Asegurar que la vida no baje de 0
    if (currentHealth < 0) {
        currentHealth = 0;
    }
    
    // Cambiar al estado de herido, pero no se inmuta si está atacando
    if(currentBoss3State != Boss3State::ATTACKING || currentBoss3State != Boss3State::JUMPING)
        changeState(static_cast<int>(Boss3State::HURT));
    
    // Activar invencibilidad
    setInvincible(true);

    sf::Vector2f attackDirection = attackPosition-position;
    setupKnockback(attackDirection, 100.0f);
    
    // Si la vida llega a 0, cambiar al estado de muerte
    if (currentHealth <= 0) {
        changeState(static_cast<int>(Boss3State::DYING));
    }
}

/**
 * Cambia al estado de ataque si el ataque no está en cooldown.
 * El daño producido al jugador se manejará en el sistema de colisiones de InGame.
 * Que la hitbox vuelva a estar inactiva se gestiona en el update.
 */
void Boss3::attack() {
    // Verificar si el ataque está en cooldown
    if (attackTimer > 0) {
        return;
    }
    
    // Cambiar al estado de ataque
    changeState(static_cast<int>(Boss3State::ATTACKING));
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

/**
 * Funcion de dibujado de los enemigos.
 */
void Boss3::render(sf::RenderWindow& window) {    
    // Dibujar el sprite del enemigo
    sprite.draw(window);

    // DEBUG
    hitbox->render(window);
    hurtbox->render(window);
}

void Boss3::update(float deltaTime, Character* player, const TileMap* tileMap) {
    updateTimers(deltaTime);
    animator->update(deltaTime);

    sf::Vector2f toPlayer = player->getPosition() - position;
    float distanceSquared = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;

    // Lógica basada en el estado actual
    switch (currentBoss3State) {

        case Boss3State::IDLE:
            if (player) {
                // Recalcular el camino solo en intervalos específicos
                if (pathUpdateTimer >= pathUpdateInterval) {
                    pathUpdateTimer = 0;
                    findPathToPlayer(player, tileMap);
                }
                if (velocity.x != 0 || velocity.y != 0)
                    changeState(static_cast<int>(Boss3State::MOVING));
            }
            break;
            
        case Boss3State::MOVING:
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
                if (distanceSquared <= 100.f * 100.f) {
                    attackPosition = player->getPosition();
                    attack();
                }
            }
            if (distanceSquared >= 175.f * 175.f && jumpTimer <= 0) {
                changeState(static_cast<int>(Boss3State::JUMPING));
            }
            
            break;
            
        case Boss3State::ATTACKING:
            // clava el objetivo en el jugador o su direccion
            if(stateTimer >= 0.5f && !targeted){
                if (distanceSquared <= 125.f * 125.f) {
                    attackPosition = player->getPosition();
                    targeted = true;
                }
                else {
                    float distance = std::sqrt(distanceSquared);
                    sf::Vector2f direction = toPlayer / distance;  // Normalizamos
                    attackPosition = position + direction * 125.f;
                    targeted = true;
                } 
            }
            // la hitbox aparece
            if (stateTimer >= 0.8f && !attacked) {  // Duración de la animación de ataque
                hitbox->setPosition(attackPosition);
                hitbox->setActive(true);
                VFXManager::getInstance().addEffect("./resources/vfx/smash.png",hitbox->getPosition(),{48 , 48},7,8.f);
                attacked = true;
            }
            if(stateTimer >= 1.f){
                hitbox->setActive(false);
            }
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 1.2f) {  // Duración de la animación de ataque
                changeState(static_cast<int>(Boss3State::IDLE));
                attacked = false;
                targeted = false;
            }
            break;

        case Boss3State::HURT:
            if(hitbox->isActive())
                hitbox->setActive(false);
            setInvincible(true);
            // La animación de daño duraría un tiempo fijo
            if (stateTimer >= 0.8f) {  // Duración de la animación de daño
                changeState(static_cast<int>(Boss3State::IDLE));
                setInvincible(false);
            }
            break;

        case Boss3State::JUMPING:
            if(!targeted){
                jumpPosition = toPlayer;
                previousMovementSpeed = movementSpeed;
                targeted = true;
            }
            if(stateTimer >= 0.7f && stateTimer < 1.25f){
                calculateVelocity(jumpPosition);
                movementSpeed = 700.f;
                move(tileMap, deltaTime);
                updateHitboxes();
            }            
            if (stateTimer >= 1.25f && !attacked) {  // Duración de la animación de ataque
                hitbox->setPosition(position);
                hitbox->setActive(true);
                VFXManager::getInstance().addEffect("./resources/vfx/smash.png",hitbox->getPosition(),{48 , 48},7,8.f);
                attacked = true;
            }
            if (stateTimer >= 1.5f) {  // Duración de la animación de ataque
                hitbox->setActive(false);
            }
            if (stateTimer >= 2.f) {  // Duración de la animación de salto
                changeState(static_cast<int>(Boss3State::IDLE));
                attacked = false;
                targeted = false;
                jumpTimer = 10.f;
                movementSpeed = previousMovementSpeed;
            }
            break;
            
        case Boss3State::DYING:
            hitbox->setActive(false);
            hurtbox->setActive(false);
            if (stateTimer >= 2.2f) {  // Duración de la animación de muerte
                changeState(static_cast<int>(Boss3State::DEAD));
            }
            break;
        case Boss3State::DEAD:
            break;
    }
    // DEBUG
    // std::cout << "Boss3,   x: " << position.x << " y: " << position.y << std::endl;
}

void Boss3::updateTimers(float deltaTime){
    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            setInvincible(false);
        }
    }

    if (jumpTimer > 0) {
        jumpTimer -= deltaTime;
    }
    
    if (attackTimer > 0) {
        attackTimer -= deltaTime;
    }
    
    stateTimer += deltaTime;
    
    pathUpdateTimer += deltaTime;
}

void Boss3::move(const TileMap* tileMap, float deltaTime){
    sf::FloatRect nextBounds = hurtbox->getGlobalBounds();
    sf::FloatRect nextXBounds = nextBounds;
    sf::FloatRect nextYBounds = nextBounds;

    // Movimiento en eje X
    nextXBounds.left += velocity.x * deltaTime;
    if (tileMap->isColliding(nextXBounds)) {
        velocity.x = 0;
    }

    // Movimiento en eje Y
    nextYBounds.top += velocity.y * deltaTime;
    if (tileMap->isColliding(nextYBounds)) {
        velocity.y = 0;
    }

    //DEBUG
    std::cout << "Velocity   X: " << velocity.x << "    Y: " << velocity.y << std::endl;

    position += velocity * deltaTime;
    sprite.move(velocity * deltaTime);
}

void Boss3::changeState(int newStateInt) {
    if (!isValidBoss3State(newStateInt)) return;
    Boss3State newState = static_cast<Boss3State>(newStateInt);

    // Si estamos cambiando a un nuevo estado, reiniciar el timer
    if (currentBoss3State != newState) {
        currentBoss3State = newState;
        stateTimer = 0.0f;
        changeAnimation(newStateInt);
        // Acciones específicas al cambiar de estado podrían ir aquí
        // Por ejemplo, cambiar la animación según el estado
    }
}

bool Boss3::isValidBoss3State(int state){
    return state >= static_cast<int>(Boss3State::IDLE) &&
           state <= static_cast<int>(Boss3State::DEAD);
}

bool Boss3::isDead() const{
    return (currentBoss3State == Boss3State::DEAD);
}