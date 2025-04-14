#include "Bow.h"
#include "Arrow.h"
#include "Character.h"
#include "SFXManager.h"
#include <iostream>
#include <cmath>

Bow::Bow(GameEngine* engine) : Weapon(engine), arrowSpeed(500.0f), abilityArrowCount(5), abilitySpreadAngle(45.0f) {
    spriteFacade.loadTexture("./resources/Weapons/Bow.png");
    name = "Bow"; // Nombre del arma
    baseDamage = 15.0f; // Daño base del arco
}

void Bow::attack(sf::Vector2f position, sf::Vector2f direction) {
    if (attackTimer <= 0.f) { // Verificar si el cooldown ha terminado
        std::cout << "Bow attack: Shooting an arrow!" << std::endl;
        increaseConsecutiveAttacks();
        // Crear una flecha y añadirla al contenedor
        arrows.emplace_back(position, direction, arrowSpeed);
        float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
        SFXManager::getInstance().addEffect("resources/sfx/bowa.wav", 60.f, pitch);

        // Reiniciar el cooldown
        attackTimer = attackCooldown;
    } else {
        std::cout << "Attack on cooldown! Time remaining: " << attackTimer << " seconds" << std::endl;
    }
}


void Bow::useAbility(sf::Vector2f position, sf::Vector2f direction) {
    if (abilityTimer <= 0.f) { // Verificar si el cooldown ha terminado
        std::cout << "Bow ability: Shooting multiple arrows in an arc!" << std::endl;

        // Asegurarse de que la dirección esté normalizada
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude != 0) {
            direction /= magnitude;
        }

        // Convertir el ángulo de dispersión a radianes
        float spreadAngleRadians = abilitySpreadAngle * (M_PI / 180.0f);

        // Calcular el ángulo inicial y el incremento entre flechas
        float startAngle = -spreadAngleRadians / 2.0f; // Ángulo inicial (mitad negativa del arco)
        float angleIncrement = spreadAngleRadians / (abilityArrowCount - 1); // Incremento entre flechas

        for (int i = 0; i < abilityArrowCount; ++i) {
            float angle = startAngle + i * angleIncrement; // Ángulo para esta flecha

            // Rotar la dirección base del personaje según el ángulo
            sf::Vector2f arrowDirection(
                direction.x * std::cos(angle) - direction.y * std::sin(angle),
                direction.x * std::sin(angle) + direction.y * std::cos(angle)
            );

            // Crear una flecha y añadirla al contenedor
            arrows.emplace_back(position, arrowDirection, arrowSpeed);
        }

        // Reiniciar el cooldown
        abilityTimer = abilityCooldown;
        consecutiveAttacks = 0;
    } else {
        std::cout << "Ability on cooldown! Time remaining: " << abilityTimer << " seconds" << std::endl;
    }
}

void Bow::update(float deltaTime, const TileMap& tileMap) {
    // Reducir los temporizadores de cooldown
    if (attackTimer > 0.f) {
        attackTimer -= deltaTime;
    }
    if (abilityTimer > 0.f) {
        abilityTimer -= deltaTime;
    }

    if(comboDamageBonus > 0.0f) {
        updateConsecutiveAttacks(deltaTime);
    }

    // Actualizar todas las flechas activas
    for (auto& arrow : arrows) {
        arrow.update(deltaTime);

        // Verificar colisión con elementos sólidos del mapa
        if (tileMap.isColliding(arrow.getBounds())) {
            arrow.markforRemoval(); // Marcar la flecha para eliminación si colisiona
        }
    }

    // Eliminar flechas que estén marcadas para eliminación
    arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
        [](const Arrow& arrow) {
            return arrow.isMarkedForRemoval();
        }),
        arrows.end());
}

/**
 * Ajusta el arco en la posicion y con la direccion del jugador.
 * Luego llama a render para dibujar el arco.
 */
void Bow::renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) {
    // Ajustar la posición del arma en función de la dirección
    if (direction.x > 0) {  // Mirando a la derecha
        spriteFacade.setPosition(position.x + 20, position.y - 34);
        spriteFacade.setRotation(45); // Sin rotación
    } else if (direction.x < 0) {  // Mirando a la izquierda
        spriteFacade.setPosition(position.x - 20, position.y + 34);
        spriteFacade.setRotation(225); // Rotar 180 grados
    } else if (direction.y < 0) {  // Mirando hacia arriba
        spriteFacade.setPosition(position.x - 34, position.y - 20);
        spriteFacade.setRotation(315); // Rotar 270 grados
    } else if (direction.y > 0) {  // Mirando hacia abajo
        spriteFacade.setPosition(position.x + 34, position.y + 20);
        spriteFacade.setRotation(135); // Rotar 90 grados
    }

    render();
}

/**
 * Dibuja el arco en la posicion y con la direccion que tiene su sprite por defecto.
 */
void Bow::render(){
    // Dibujar el sprite del arma
    spriteFacade.draw(engine->getWindow());
    for(const Arrow& arrow : arrows) {
        arrow.draw(engine->getWindow()); // Dibujar cada flecha
    }
}

void Bow::increaseAbilityArrowCount(int count) {
    abilityArrowCount += count; // Incrementar el número de flechas de la habilidad
}

void Bow::increaseArrowSpeed(float speed) {
    arrowSpeed += speed; // Incrementar la velocidad de las flechas
}