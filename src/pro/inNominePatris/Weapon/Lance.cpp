#include "Lance.h"
#include <iostream>
#include "Character.h"
#include "../hboxes/Hitbox.h"
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <SFXManager.h>

Lance::Lance(GameEngine* engine) 
:   Weapon(engine), 
    attackHitbox(std::make_shared<Hitbox>(sf::Vector2f(200.f, 30.f), sf::Vector2f(0.f, 0.f) )), 
    attackCooldown(0.7f), 
    attackTimer(0.f), 
    isPortalDropped(false), 
    PortalRange(300.0f), 
    portal(), 
    pinchSprite("./resources/Pinch_Animation_Lance.png"),
    pinchAnimation(pinchSprite) 
{
    spriteFacade.loadTexture("./resources/Weapons/lance.png"); // Cargar textura usando el Façade
    name = "Lance"; // Nombre del arma

    pinchAnimation.addAnimation(
        "pinch",
        5,
        sf::Vector2i(0, 0),
        sf::Vector2i(96, 32),
        false
    );
    pinchAnimation.setAnimationEndCallback([this]() {
        isAnimating = false; // Desactivar la animación al finalizar
    });

    baseDamage = 20.0f;
}

void Lance::createHitbox(sf::Vector2f position, sf::Vector2f direction) {
    sf::Vector2f offset;
    sf::Vector2f size;
    
    // Calcular el ángulo para la dirección de apuntado
    float angle = std::atan2(direction.y, direction.x);
    
    // Calcular el offset basado en la dirección normalizada
    float offsetDistance = 70.f;
    offset.x = direction.x * offsetDistance;
    offset.y = direction.y * offsetDistance;
    
    // Ajustar el tamaño de la hitbox según la dirección
    if (std::abs(direction.x) > std::abs(direction.y)) {
        // Más horizontal que vertical
        size = sf::Vector2f(110.f, 30.f);
    } else {
        // Más vertical que horizontal
        size = sf::Vector2f(30.f, 110.f);
    }

    attackHitbox->setSize(size);
    attackHitbox->setPosition(position + offset);
    attackHitbox->setActive(true);
}

void Lance::attack(sf::Vector2f position, sf::Vector2f direction) {
    if (attackTimer <= 0.f) {
        createHitbox(position, direction); // Usar la dirección de apuntado
        attackTimer = attackCooldown;     // Reiniciar el temporizador de ataque
        increaseConsecutiveAttacks(); // Aumentar el contador de ataques consecutivos
        
        // Configurar la posición inicial del ataque
        sf::Vector2f pinchOffset;
        float rotation = 0.f;
        
        // Calcular el ángulo de rotación basado en la dirección de apuntado
        float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
        rotation = angle;
        
        // Ajustar el offset basado en el ángulo
        pinchOffset.x = direction.x * 30.f;
        pinchOffset.y = direction.y * 30.f;

        // Configurar la posición y rotación del ataque
        pinchSprite.setPosition(position.x + pinchOffset.x, position.y + pinchOffset.y);
        pinchSprite.setRotation(rotation);

        // Reproducir la animación del ataque
        pinchAnimation.play("pinch", 12.0f, false); // 12 FPS, no en bucle
        isAnimating = true;
        float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
        SFXManager::getInstance().addEffect("resources/sfx/spear.wav", 60.f, pitch);

        float damage = calculateDamage(); // Calcular el daño
        std::cout << "Lance attack: Dealt " << damage << " damage!" << std::endl;
    }
}

// REIMPLEMENTAR
void Lance::useAbility(sf::Vector2f characterPosition, sf::Vector2f mousePosition) {
    if (abilityTimer > 0.f) {
        std::cout << "Ability on cooldown! Time remaining: " << abilityTimer << " seconds" << std::endl;
        return;
    }

    if (!isPortalDropped) {
        // Calcular la distancia entre el personaje y la posición del ratón
        float distance = std::sqrt(std::pow(mousePosition.x - characterPosition.x, 2) +
                                   std::pow(mousePosition.y - characterPosition.y, 2));

        if (distance <= PortalRange) {
            std::cout << "Dropping Portal at: " << mousePosition.x << ", " << mousePosition.y << std::endl;
            portal.setPosition(mousePosition);
            portal.setVisible(true);
            isPortalDropped = true;

            // Reiniciar el cooldown
            abilityTimer = abilityCooldown;
        } else {
            std::cout << "Portal placement out of range! Distance: " << distance << " - PortalRange: " << PortalRange << std::endl;
        }
    } else {
        std::cout << "Portal already placed. Ready to teleport!" << std::endl;
    }
}

const sf::Vector2f& Lance::teleportToPortal() {
    portal.setVisible(false);
    isPortalDropped = false;
    return portal.getPosition();
    consecutiveAttacks = 0;
}

Portal::Portal() : position(0.f, 0.f), visible(false) {
    loadAnimationFrames();
    sprite.setScale(1.0f, 1.0f);

    if (animationFrames.empty()) {
        std::cerr << "Warning: Portal created without valid animation frames!" << std::endl;
    }
}

Portal::Portal(const sf::Vector2f& pos) : position(pos), visible(false) {
    loadAnimationFrames();
    sprite.setScale(1.0f, 1.0f);
    setPosition(pos);

    if (animationFrames.empty()) {
        std::cerr << "Warning: Portal created without valid animation frames!" << std::endl;
    }
}

void Portal::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position.x - sprite.getGlobalBounds().width / 2, 
    position.y - sprite.getGlobalBounds().height / 2);
}

const sf::Vector2f& Portal::getPosition() const {
    return position;
}

void Portal::draw(sf::RenderWindow& window) {
    if (visible && sprite.getTexture() != nullptr) {
        window.draw(sprite);
    }
}

void Lance::DrawPortal(sf::RenderWindow& window) {
    portal.draw(window);
}

void Lance::update(float deltaTime) {
    if (attackTimer > 0.f) {
        attackTimer -= deltaTime;
        if (attackTimer <= 0.f) {
            attackHitbox->setActive(false); // Desactivar la hitbox después de un ataque
            dealtDamage = false;          // Reiniciar el estado de daño
        }
    }

    // Reducir el temporizador de la habilidad
    if (abilityTimer > 0.f) {
        abilityTimer -= deltaTime;
    }

    if(isAnimating){
        pinchAnimation.update(deltaTime);
    }

    if(comboDamageBonus > 0.0f) {
        updateConsecutiveAttacks(deltaTime);
    }
}

void Lance::PortalUpdate(float deltaTime) {
    portal.update(deltaTime);
}

void Portal::setVisible(bool visible) {
    this->visible = visible;
}

bool Portal::isVisible() const {
    return visible;
}

void Portal::loadAnimationFrames() {
    for (int i = 1; i <= 7; i++) {
        sf::Texture texture;
        std::string filePath = "./resources/portal-animation/portal1_frame_" + std::to_string(i) + ".png";
        if (texture.loadFromFile(filePath)) {
            animationFrames.push_back(texture);
        } else {
            std::cerr << "Error loading " << filePath << std::endl;
        }
    }

    // Verificar si se cargaron frames correctamente
    if (!animationFrames.empty()) {
        sprite.setTexture(animationFrames[0]); // Usar el primer frame por defecto
    } else {
        std::cerr << "No animation frames loaded for portal!" << std::endl;
    }
}

void Portal::update(float deltaTime) {
    if (!visible || animationFrames.empty()) return;

    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % animationFrames.size();
        sprite.setTexture(animationFrames[currentFrame]);
    }
}


/**
 * Ajusta la lanza en la posicion y con la direccion del jugador.
 * Luego llama a render para dibujar la lanza.
 */
void Lance::renderOnPlayer(sf::Vector2f position, sf::Vector2f direction,sf::RenderWindow& window) {
    // Normalize the direction vector
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Calculate angle in degrees
    float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
    
    // Set the origin to the center of the lance sprite for proper rotation
    sf::Vector2u textureSize = {64,64};
    spriteFacade.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
    
    // Distance from player to lance
    float lanceDistance = 25.f;
    
    // Calculate new position
    sf::Vector2f lancePosition = position + direction * lanceDistance;
    
    // Position the lance
    spriteFacade.setPosition(lancePosition.x, lancePosition.y);
    spriteFacade.setRotation(angle);
    
    render(window);
}

/**
 * Dibuja la lanza en la posicion y con la direccion que tiene su sprite por defecto.
 */
void Lance::render(sf::RenderWindow& window){
    // Dibujar el sprite del arma
    spriteFacade.draw(engine->getWindow());
    //attackHitbox.render(engine->getWindow()); // Dibujar la hitbox de ataque
    if (isPortalDropped) {
        portal.draw(engine->getWindow());
    }

    if (isAnimating) {
        pinchSprite.draw(engine->getWindow());
    }
}

std::shared_ptr<Hitbox> Lance::getAttackHitbox() const {
    return attackHitbox;
}

void Lance::increasePortalRange(float range) {
    PortalRange += range;
    std::cout << "Portal range increased!" << std::endl;
}

void Lance::decreaseAttackCooldown(float cooldown) {
    attackCooldown -= cooldown;
    if (attackCooldown < 0.1f) attackCooldown = 0.1f; // Limitar el cooldown mínimo
    std::cout << "Attack cooldown decreased!" << std::endl;
}

void Lance::increaseAttackDamage(float damage) {
    baseDamage += damage;
    std::cout << "Attack damage increased!" << std::endl;
}
