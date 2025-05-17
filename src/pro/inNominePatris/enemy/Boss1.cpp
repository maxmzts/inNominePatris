#include "Boss1.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>
#include <EnemyManager.h>
#include "Boss1Minion.h"

Boss1::Boss1(const sf::Vector2f& startPosition, int DropKarmaPoints)
    : Enemy(
        "Boss1", 
        150.f, 
        80.f, 
        startPosition, 
        "resources/enemies/boss1.png"),
        spawnTimer(5.f),
        attacked(false),
        targeted(false), 
        currentBoss1State(Boss1State::IDLE)
{
    sprite.setTextureRect(sf::IntRect(0,0, 220, 112));
    sprite.setOrigin(220/2, 112/2);
    sprite.setScale(1.5f,1.5f);
    hurtbox->setOffset({-8, 60.f});
    // hurtbox->setSize();
    hitbox->setSize({45,45});
    loadAnimations();
    attackCooldown = 4.f;
    KarmaPoints = DropKarmaPoints; 
}

/**
 * Carga las animaciones de este enemigo.
 */
void Boss1::loadAnimations() {
    sf::Vector2i resolution = sf::Vector2i(220, 112);
    animator->addAnimation("idle",      10, sf::Vector2i(0, 0),                 resolution, true);
    animator->addAnimation("moving",    12, sf::Vector2i(0, resolution.y),      resolution, true);
    animator->addAnimation("attacking", 16, sf::Vector2i(0, resolution.y*2),    resolution, false);
    animator->addAnimation("dying",     17, sf::Vector2i(0, resolution.y*3),    resolution, false);
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
        case Boss1State::SPAWNING:
            animator->play("idle", 8.f);
            break;
        case Boss1State::HURT:
            animator->play("idle", 0.f, false);
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
    
    // Cambiar al estado de herido, pero no se inmuta si está atacando
    if(currentBoss1State != Boss1State::ATTACKING)
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
    // Dibujar el sprite del enemigo
    sprite.draw(window);

    // DEBUG
    hitbox->render(window);
    hurtbox->render(window);
}

void Boss1::update(float deltaTime, Character* player, const TileMap* tileMap) {
    updateTimers(deltaTime);
    animator->update(deltaTime);

    // parpadea si spawnea
    if(currentBoss1State == Boss1State::SPAWNING) {
        r += std::max(1.f, deltaTime) * t * 5;
        g += std::max(1.f, deltaTime) * t * 5;
        b += std::max(1.f, deltaTime) * t * 5;

        // std::cout << "r: " << r << "   g: " << g <<  "   b: " << b << "   t: " << t << std::endl; 
    
        if(r <= 75 || g <= 75 || b <= 75){
            t = 1;
        } else if (r >= 255 || g >= 255 || b >= 255){
            t = -1;
        }
    
        // Limita los valores entre 0 y 255 para evitar overflow o underflow
        r = std::max(0, std::min(255, r));
        g = std::max(0, std::min(255, g));
        b = std::max(0, std::min(255, b));
        sprite.setColor(sf::Color(r, g, b, 255)); 
    }
    else {
        r=255;g=255;b=255;
        sprite.setColor(sf::Color(255, 255, 255, 255));
    }

    sf::Vector2f toPlayer = player->getPosition() - position;
    float distanceSquared = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;

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
                if (distanceSquared <= 100.f * 100.f) {
                    attackPosition = player->getPosition();
                    attack();
                }
            } // spawnear murcielago
            else if (player && spawnTimer <= 0)
                changeState(static_cast<int>(Boss1State::SPAWNING));
            
            break;
            
        case Boss1State::ATTACKING:
            // clava el objetivo en el jugador o su direccion
            if(stateTimer >= 1 && !targeted){
                if (distanceSquared <= 100.f * 100.f) {
                    attackPosition = player->getPosition();
                    targeted = true;
                }
                else {
                    float distance = std::sqrt(distanceSquared);
                    sf::Vector2f direction = toPlayer / distance;  // Normalizamos
                    attackPosition = position + direction * 100.f;
                    targeted = true;
                }
                
            }
            // la hitbox aparece
            if (stateTimer >= 1.375f && !attacked) {  // Duración de la animación de ataque
                hitbox->setPosition(attackPosition);
                hitbox->setActive(true);
                VFXManager::getInstance().addEffect("./resources/vfx/smash.png",hitbox->getPosition(),{48 , 48},7,8.f);
                attacked = true;
            }
            if(stateTimer >= 1.5f){
                hitbox->setActive(false);
            }
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 2.f) {  // Duración de la animación de ataque
                changeState(static_cast<int>(Boss1State::IDLE));
                attacked = false;
                targeted = false;
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
            spawnIntervalTimer += deltaTime;
            // Verifica si ya ha spawneado todos los enemigos
            if (spawnedEnemiesCount < 6) {
                // Espera 1 segundo entre cada spawn
                if (spawnIntervalTimer >= 1.f) {
                    spawn(); // Spawnea un enemigo
                    spawnedEnemiesCount++;
                    spawnIntervalTimer = 0.f; // Reinicia el temporizador de intervalo
                }
            } else {
                // Ha terminado de spawnear los 6 enemigos
                changeState(static_cast<int>(Boss1State::IDLE));
                spawnTimer = 10.f;
                spawnedEnemiesCount = 0; // Reinicia el contador para el futuro
            }
            break;
        
            
        case Boss1State::DYING:
            hitbox->setActive(false);
            hurtbox->setActive(false);
            if (stateTimer >= 2.f) {  // Duración de la animación de muerte
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

void Boss1::move(const TileMap* tileMap, float deltaTime){
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
    position += velocity * deltaTime;
    sprite.move(velocity * deltaTime);
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
        std::make_shared<Boss1Minion>((position/16.f), 0)
    );
}