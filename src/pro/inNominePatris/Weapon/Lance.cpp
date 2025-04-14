#include "Lance.h"
#include <iostream>
#include "Character.h"
#include "../hboxes/Hitbox.h"
#include <SFML/Window/Mouse.hpp>
#include <cmath>

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

    if (direction.x > 0) { // Derecha
        offset = sf::Vector2f(70.f, 0.f);
        size = sf::Vector2f(110.f, 30.f); // Hitbox horizontal
    } else if (direction.x < 0) { // Izquierda
        offset = sf::Vector2f(-70.f, 0.f);
        size = sf::Vector2f(110.f, 30.f); // Hitbox horizontal
    } else if (direction.y > 0) { // Abajo
        offset = sf::Vector2f(0.f, 70.f);
        size = sf::Vector2f(30.f, 110.f); // Hitbox vertical
    } else if (direction.y < 0) { // Arriba
        offset = sf::Vector2f(0.f, -70.f);
        size = sf::Vector2f(30.f, 110.f); // Hitbox vertical
    }
    attackHitbox->setSize(size); // Ajustar el tamaño de la hitbox
    attackHitbox->setPosition(position + offset); // Ajustar la posición de la hitbox
    attackHitbox->setActive(true); // Activar la hitbox
}

void Lance::attack(sf::Vector2f position, sf::Vector2f direction) {
    if (attackTimer <= 0.f) {
        createHitbox(position, direction); // Crear la hitbox del ataque
        attackTimer = attackCooldown;     // Reiniciar el temporizador de ataque
        increaseConsecutiveAttacks(); // Aumentar el contador de ataques consecutivos
        // Configurar la posición inicial del ataque
        sf::Vector2f pinchOffset;
        float rotation = 0.f;

        if (direction.x > 0) { // Derecha
            pinchOffset = sf::Vector2f(30.f, -15.f); // Desplazar hacia la derecha
            rotation = 0.f;
        } else if (direction.x < 0) { // Izquierda
            pinchOffset = sf::Vector2f(-30.f, 15.f); // Desplazar hacia la izquierda
            rotation = 180.f;
        } else if (direction.y > 0) { // Abajo
            pinchOffset = sf::Vector2f(15.f, 30.f); // Desplazar hacia abajo
            rotation = 90.f;
        } else if (direction.y < 0) { // Arriba
            pinchOffset = sf::Vector2f(-15.f, -30.f); // Desplazar hacia arriba
            rotation = -90.f;
        }

        // Configurar la posición y rotación del ataque
        pinchSprite.setPosition(position.x + pinchOffset.x, position.y + pinchOffset.y);
        pinchSprite.setRotation(rotation);

        // Reproducir la animación del ataque
        pinchAnimation.play("pinch", 12.0f, false); // 12 FPS, no en bucle
        isAnimating = true;

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
    // Ajustar la posición del arma en función de la dirección
    if (direction.x > 0) {  // Mirando a la derecha
        spriteFacade.setPosition(position.x + 20, position.y-20);
        spriteFacade.setRotation(0); // Sin rotación
    } else if (direction.x < 0) {  // Mirando a la izquierda
        spriteFacade.setPosition(position.x - 20, position.y+20);
        spriteFacade.setRotation(180); // Rotar 180 grados
    } else if (direction.y < 0) {  // Mirando hacia arriba
        spriteFacade.setPosition(position.x-20, position.y - 20);
        spriteFacade.setRotation(270); // Rotar 270 grados
    } else if (direction.y > 0) {  // Mirando hacia abajo
        spriteFacade.setPosition(position.x+20, position.y + 20);
        spriteFacade.setRotation(90); // Rotar 90 grados
    }

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
