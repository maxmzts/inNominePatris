#include "Boss2.h"
#include "VFXManager.h"
#include "SFXManager.h"
#include "EnemyManager.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Boss2::Boss2(const sf::Vector2f& startPosition, int DropKarmaPoints)
    : Enemy("Boss2", 1.f, 128.f, startPosition, "resources/enemies/boss2.png"),
      currentBoss2State(Boss2State::IDLE),
      idleTimer(0.f),
      moveTimer(0.f),
      attackCooldown(2.f),
      attackTimer(0.f),
      invincibilityDuration(1.5f),
      meleeRange(100.f),        // Aumentado para mejor detección
      rangedRange(400.f),       // Aumentado para mejor detección
      attackPattern(0),
      projectileSpeed(220.f),   // Aumentado para proyectiles más efectivos
      attacked(false),
      stateTimer(0.f)           // Inicializado explícitamente
{
    std::cout << "[Boss2] Constructor llamado en posición: " << startPosition.x << ", " << startPosition.y << std::endl;
    loadAnimations();
    KarmaPoints = DropKarmaPoints;
    
    // HITBOX - ajustada para mejor colisión
    hitbox->setSize(sf::Vector2f(100.f, 100.f));
    hitbox->setOffset(sf::Vector2f(0.f, 37.f));
    hitbox->setActive(true);    // Aseguramos que la hitbox esté activa

    // HURTBOX - ajustada para mejor detección de daño
    hurtbox->setSize(sf::Vector2f(120.f, 120.f));
    hurtbox->setOffset(sf::Vector2f(0.f, 37.f));
    hurtbox->setActive(true);   // Aseguramos que la hurtbox esté activa

    // ATTACK HITBOX - rediseñada para mejor detección
    attackHitbox = new Hitbox(sf::Vector2f(150.f, 120.f), sf::Vector2f(0.f, 0.f), sf::Color(0, 255, 0, 128));
    attackHitbox->setActive(true);
    
    // Establecer valores iniciales
    facingRight = true;
    isInvincible = false;
    invincibilityTimer = 0.f;
}

Boss2::~Boss2() {
    std::cout << "[Boss2] Destructor llamado" << std::endl;
    delete attackHitbox;
}

void Boss2::loadAnimations() {
    std::cout << "[Boss2] Cargando animaciones..." << std::endl;
    // Definir animaciones usando el spritesheet
    animator->addAnimation("idle", 8, sf::Vector2i(0, 0), sf::Vector2i(264, 177), true);
    animator->addAnimation("moving", 12, sf::Vector2i(0, 177), sf::Vector2i(264, 177), true);
    animator->addAnimation("attack_melee", 13, sf::Vector2i(0, 354), sf::Vector2i(264, 177), false);
    animator->addAnimation("attack_ranged", 9, sf::Vector2i(0, 531), sf::Vector2i(264, 177), false);
    animator->addAnimation("dying", 18, sf::Vector2i(0, 708), sf::Vector2i(264, 177), false);
    
    // Establecer animación inicial
    animator->play("idle", 8.f);

    // Centrado correcto del sprite
    sprite.setOrigin(132.f, 88.5f);
}

void Boss2::changeAnimation(int newStateInt) {
    std::cout << "[Boss2] Cambiando animación a estado: " << newStateInt << std::endl;
    Boss2State newState = static_cast<Boss2State>(newStateInt);
    switch (newState) {
        case Boss2State::IDLE:
            animator->play("idle", 8.f);
            break;
        case Boss2State::MOVING:
            animator->play("moving", 12.f);
            break;
        case Boss2State::ATTACKING_MELEE:
            animator->play("attack_melee", 16.f, false);
            break;
        case Boss2State::ATTACKING_RANGED:
            animator->play("attack_ranged", 12.f, false);
            break;
        case Boss2State::DYING:
            animator->play("dying", 8.f, false);
            // SFXManager::getInstance()->playSFX("boss_death");
            // VFXManager::getInstance()->createEffect("death_explosion", position);
            break;
        case Boss2State::DEAD:
            // No hay animación para este estado
            break;
        case Boss2State::HURT:
            // En caso de daño, mostrar animación de idle con efecto visual
            animator->play("idle", 8.f);
            break;
    }
}

void Boss2::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    std::cout << "[Boss2] Recibe daño: " << damage << " en posición: " << attackPosition.x << ", " << attackPosition.y << std::endl;
    if (isInvincible || currentBoss2State == Boss2State::DYING || currentBoss2State == Boss2State::DEAD)
        return;

    currentHealth -= damage;
    if (currentHealth < 0) currentHealth = 0;

    //Efectos de sonido y visuales al recibir daño
    try {
        //SFXManager::getInstance()->playSFX("boss_hurt");
        VFXManager::getInstance().addEffect("./resources/vfx/blood.png",hitbox->getPosition(),{45 , 45},5,8.f);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] No se pudo reproducir efectos: " << e.what() << std::endl;
    }
    
    setInvincible(true);
    invincibilityTimer = invincibilityDuration;

    // Knockback solo si no está atacando
    if (currentBoss2State != Boss2State::ATTACKING_MELEE && currentBoss2State != Boss2State::ATTACKING_RANGED) {
        sf::Vector2f knockbackDir = position - attackPosition;
        float len = std::sqrt(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);
        if (len > 0) knockbackDir /= len;
        setupKnockback(knockbackDir, 150.f);
    }

    if (currentHealth <= 0) {
        std::cout << "[Boss2] ¡Muerto!" << std::endl;
        changeState(static_cast<int>(Boss2State::DYING));
    } else {
        std::cout << "[Boss2] Cambia a estado HURT" << std::endl;
        changeState(static_cast<int>(Boss2State::HURT));
    }
}

void Boss2::attackMelee() {
    std::cout << "[Boss2] Ataque melee ejecutado" << std::endl;

    // Posicionar correctamente la hitbox de ataque según la dirección
    float xOffset = facingRight ? 120.f : -120.f;
    sf::Vector2f hitboxPos = position + sf::Vector2f(xOffset, 0.f);
    
    // Configurar y activar hitbox de ataque
    attackHitbox->setActive(true);
    attackHitbox->setPosition(hitboxPos);
    
    std::cout << "[Boss2] Hitbox de ataque activada en " << hitboxPos.x << ", " << hitboxPos.y << std::endl;
    
    // Efectos de sonido y visuales
    try {
        float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
        SFXManager::getInstance().addEffect("./resources/sfx/blood_slash.wav", 100.f, pitch);        
        //VFXManager::getInstance()->createEffect("melee_impact", hitboxPos);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] No se pudo reproducir efectos: " << e.what() << std::endl;
    }
    
    // Verificar colisión con el jugador directamente
    Character* player = Character::getInstance();
    if (player && attackHitbox->getGlobalBounds().intersects(player->getHurtbox()->getGlobalBounds())) {
        std::cout << "[Boss2] Ataque melee impacta al jugador" << std::endl;
        player->takeDamage(50.f); // Aplicar 50 de daño
    }

    attacked = true;
    attackTimer = attackCooldown;
}

void Boss2::attackRanged(const sf::Vector2f& playerPos) {
    std::cout << "[Boss2] Ataque a distancia hacia: " << playerPos.x << ", " << playerPos.y << std::endl;
    
    // Calcular dirección del proyectil
    sf::Vector2f direction = playerPos - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) direction /= length;
    
    // Ajustar la posición inicial del proyectil para que salga de la mano/boca del boss
    sf::Vector2f projectilePos = position + sf::Vector2f(facingRight ? 10.f : -10.f, -20.f);
    
    // Check if EnemyManager instance exists
    if (!EnemyManager::getInstance()) {
        std::cerr << "[ERROR] EnemyManager instance is null!" << std::endl;
        return;
    }
    
    std::cout << "[Boss2] Creando proyectil en posición: " << projectilePos.x << ", " << projectilePos.y 
              << " con velocidad: " << (direction.x * projectileSpeed) << ", " << (direction.y * projectileSpeed) << std::endl;
    
    std::cout << "[Boss2] Posicion del Boss: " << position.x << ", " << position.y << std::endl;

    // Crear proyectil a través del EnemyManager
    try {
        EnemyManager::getInstance()->createProjectile(
            projectilePos, 
            direction * projectileSpeed, 
            40.f,    // Aumentado el daño del proyectil
            3.0f     // Tiempo de vida del proyectil
        );
        
        // // Efectos de sonido y visuales
        // SFXManager::getInstance()->playSFX("boss2_ranged_attack");
        // VFXManager::getInstance()->createEffect("magic_cast", projectilePos);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] No se pudo crear proyectil: " << e.what() << std::endl;
    }
    
    attacked = true;
    attackTimer = attackCooldown;
}

void Boss2::calculateVelocity(const sf::Vector2f& direction) {
    sf::Vector2f dir = direction;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0) dir /= len;
    
    // Actualizar la dirección a la que mira el boss
    if (dir.x > 0) {
        facingRight = true;
        sprite.setScale(1.f, 1.f);
    } else if (dir.x < 0) {
        facingRight = false;
        sprite.setScale(-1.f, 1.f);
    }
    
    velocity = dir * movementSpeed;
}

void Boss2::render(sf::RenderWindow& window) {
    // Renderizar el sprite
    sprite.draw(window);
    
    // Debug: visualizar hitboxes
    hitbox->render(window);
    hurtbox->render(window);
    
    if (attackHitbox->isActive()) {
        attackHitbox->render(window);
    }
}

void Boss2::updateTimers(float deltaTime) {
    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            setInvincible(false);
        }
    }
    
    if (attackTimer > 0) {
        attackTimer -= deltaTime;
    }
}

void Boss2::update(float deltaTime, Character* player, const TileMap* tileMap) {
    updateTimers(deltaTime);
    animator->update(deltaTime);
    
    // Si el boss está muerto, no hacer nada más
    if (currentBoss2State == Boss2State::DEAD) {
        return;
    }

    // Actualizar la posición del sprite según la del enemigo
    sprite.setPosition(position.x, position.y);
    
    // Aplicar parpadeo durante invulnerabilidad
    if (isInvincible) {
        int blinkInterval = static_cast<int>(invincibilityTimer * 10) % 2;
        sprite.setColor(blinkInterval ? sf::Color(255, 255, 255, 128) : sf::Color::White);
    } else {
        sprite.setColor(sf::Color::White);
    }

    // Lógica específica para cada estado
    switch (currentBoss2State) {
        case Boss2State::IDLE: {
            idleTimer -= deltaTime;
            if (idleTimer <= 0) {
                if (player) {
                    sf::Vector2f toPlayer = player->getPosition() - position;
                    float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
                    
                    // Elegir acción según la distancia al jugador
                    if (dist < meleeRange) {
                        changeState(static_cast<int>(Boss2State::ATTACKING_MELEE));
                    } else if (dist < rangedRange) {
                        // 50% probabilidad de moverse, 50% de ataque a distancia (más agresivo)
                        if (static_cast<float>(rand()) / RAND_MAX < 0.5f) {
                            changeState(static_cast<int>(Boss2State::MOVING));
                        } else {
                            changeState(static_cast<int>(Boss2State::ATTACKING_RANGED));
                        }
                    } else {
                        // 80% de moverse hacia el jugador si está lejos
                        if (static_cast<float>(rand()) / RAND_MAX < 0.8f) {
                            changeState(static_cast<int>(Boss2State::MOVING));
                        } else {
                            changeState(static_cast<int>(Boss2State::ATTACKING_RANGED));
                        }
                    }
                } else {
                    // Si no hay jugador, simplemente moverse aleatoriamente
                    changeState(static_cast<int>(Boss2State::MOVING));
                }
            }
            break;
        }
        
        case Boss2State::MOVING: {
            if (player) {
                sf::Vector2f toPlayer = player->getPosition() - position;
                float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
                
                if (dist < meleeRange) {
                    // Si estamos cerca del jugador, atacar cuerpo a cuerpo
                    changeState(static_cast<int>(Boss2State::ATTACKING_MELEE));
                } else {
                    // Calcular dirección hacia el jugador y moverse
                    calculateVelocity(toPlayer);
                    move(tileMap, deltaTime);
                    updateHitboxes();
                    
                    // Mayor probabilidad de ataque a distancia mientras se mueve
                    if (dist < rangedRange && attackTimer <= 0 && static_cast<float>(rand()) / RAND_MAX < 0.05f) {
                        changeState(static_cast<int>(Boss2State::ATTACKING_RANGED));
                    }
                    
                    // Tiempo máximo en estado de movimiento antes de decidir qué hacer
                    moveTimer += deltaTime;
                    if (moveTimer > 3.0f) {  // Reducido para más dinámica
                        // 30% de probabilidad de descansar, 70% de seguir persiguiendo
                        if (static_cast<float>(rand()) / RAND_MAX < 0.3f) {
                            changeState(static_cast<int>(Boss2State::IDLE));
                        } else {
                            moveTimer = 0.f;  // Reiniciar el timer y seguir persiguiendo
                        }
                    }
                }
            } else {
                // Si no hay jugador, moverse aleatoriamente
                moveTimer += deltaTime;
                if (moveTimer > 2.0f) {
                    changeState(static_cast<int>(Boss2State::IDLE));
                }
            }
            break;
        }
        
        case Boss2State::ATTACKING_MELEE: {
            if (!attacked && stateTimer >= 0.7f && stateTimer <= 0.9f) {
                attackMelee();
            } else if (stateTimer > 1.2f) {
                // Desactivar el hitbox después del ataque
                attackHitbox->setActive(false);
            }
            
            // Cuando termina el tiempo del ataque melee
            if (stateTimer >= 1.0f) {
                attacked = false;
                attackHitbox->setActive(false);
                
                // 70% de probabilidad de hacer un segundo ataque melee si el jugador sigue cerca
                if (player) {
                    sf::Vector2f toPlayer = player->getPosition() - position;
                    float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
                    
                    if (dist < meleeRange && static_cast<float>(rand()) / RAND_MAX < 0.7f) {
                        changeState(static_cast<int>(Boss2State::ATTACKING_MELEE));
                        return;
                    }
                }
                
                changeState(static_cast<int>(Boss2State::IDLE));
            }
            break;
        }
        
        case Boss2State::ATTACKING_RANGED: {
            // Frame específico para disparar el proyectil
                if (!attacked && stateTimer >= 0.6f) {
                if (player) {
                    attackRanged(player->getPosition());
                    attacked = true;
                    // Prepara el segundo disparo
                    secondRangedAttackPending = true;
                    secondRangedAttackTimer = 0.f;
                }
            }

            // Segundo disparo 0.4s después
            if (secondRangedAttackPending) {
                secondRangedAttackTimer += deltaTime;
                if (secondRangedAttackTimer >= 0.3f) {
                    if (player) {
                        attackRanged(player->getPosition());
                    }
                    secondRangedAttackPending = false;
                }
            }
            
            // Cuando termina el tiempo del ataque a distancia
            if (stateTimer >= 1.0f) {
                attacked = false;
                
                // Mayor probabilidad de moverse después de un ataque a distancia
                if (static_cast<float>(rand()) / RAND_MAX < 0.8f) {
                    changeState(static_cast<int>(Boss2State::MOVING));
                } else {
                    changeState(static_cast<int>(Boss2State::IDLE));
                }
            }
            break;
        }
        
        case Boss2State::HURT: {
            // Aplicar knockback y procesar estado de daño
            updateHitboxes();
            
            // Cuando termina el tiempo de invulnerabilidad o animación de daño
            if (stateTimer >= (invincibilityDuration/2.f)) {
                if (player) {
                    sf::Vector2f toPlayer = player->getPosition() - position;
                    float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
                    
                    // Después de recibir daño, elegir entre contraatacar o retirarse
                    if (dist < meleeRange && static_cast<float>(rand()) / RAND_MAX < 0.7f) {
                        changeState(static_cast<int>(Boss2State::ATTACKING_MELEE));
                    } else if (dist < rangedRange && static_cast<float>(rand()) / RAND_MAX < 0.6f) {
                        changeState(static_cast<int>(Boss2State::ATTACKING_RANGED));
                    } else {
                        changeState(static_cast<int>(Boss2State::MOVING));
                    }
                } else {
                    changeState(static_cast<int>(Boss2State::IDLE));
                }
            }
            break;
        }
        
        case Boss2State::DYING: {
            // Desactivar hitboxes durante la muerte
            hitbox->setActive(false);
            hurtbox->setActive(false);
            attackHitbox->setActive(false);
            float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
            SFXManager::getInstance().addEffect("./resources/sfx/Boss2_die.wav", 100.f, pitch);        
            // Cuando termina la animación de muerte
            if (stateTimer >= 2.2f) {
                changeState(static_cast<int>(Boss2State::DEAD));
                
                // Liberar recursos y generar recompensas
                try {
                    Character::getInstance()->addKarma(KarmaPoints);
                    // VFXManager::getInstance()->createEffect("karma_gain", position);
                    //SFXManager::getInstance()->playSFX("karma_collect");
                } catch (const std::exception& e) {
                    std::cerr << "[ERROR] No se pudo otorgar karma: " << e.what() << std::endl;
                }
            }
            break;
        }
        
        case Boss2State::DEAD: {
            // No hacer nada en estado muerto
            break;
        }
    }
    
    stateTimer += deltaTime;
}

void Boss2::updateHitboxes() {
    // Actualizar posición de hitboxes
    hitbox->setPosition(position);
    hurtbox->setPosition(position);
    
    // Si el ataque está activo, actualizar su posición según la dirección
    if (attackHitbox->isActive()) {
        float xOffset = facingRight ? 120.f : -120.f;
        attackHitbox->setPosition(position + sf::Vector2f(xOffset, 0.f));
    }
}

void Boss2::changeState(int newStateInt) {
    Boss2State newState = static_cast<Boss2State>(newStateInt);
    if (currentBoss2State != newState) {
        std::cout << "[Boss2] Cambio de estado: " << static_cast<int>(currentBoss2State) << " -> " << newStateInt << std::endl;
        currentBoss2State = newState;
        stateTimer = 0.f;
        attacked = false;
        
        switch (newState) {
            case Boss2State::IDLE:
                idleTimer = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;  // Reducido para mayor actividad
                break;
                
            case Boss2State::MOVING:
                moveTimer = 0.f;
                break;
                
            case Boss2State::ATTACKING_MELEE:
                attackHitbox->setActive(false);  // Se activará en el momento preciso
                break;
                
            case Boss2State::ATTACKING_RANGED:
                break;
                
            case Boss2State::HURT:
                break;
                
            case Boss2State::DYING:
                // try {
                //     SFXManager::getInstance()->playSFX("boss_death_start");
                // } catch (const std::exception& e) {
                //     std::cerr << "[ERROR] No se pudo reproducir sonido: " << e.what() << std::endl;
                // }
                break;
                
            case Boss2State::DEAD:
                // try {
                //     SFXManager::getInstance()->playSFX("boss_death_end");
                // } catch (const std::exception& e) {
                //     std::cerr << "[ERROR] No se pudo reproducir sonido: " << e.what() << std::endl;
                // }
                break;
        }
        
        changeAnimation(newStateInt);
    }
}

bool Boss2::isDead() const {
    return currentBoss2State == Boss2State::DEAD;
}

int Boss2::getKarmaPoints() const {
    return KarmaPoints;
}