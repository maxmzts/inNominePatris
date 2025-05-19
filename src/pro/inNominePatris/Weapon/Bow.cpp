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
        // Asegurarse de que la dirección esté normalizada
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude != 0) {
            direction /= magnitude;
        }
        
      // std::cout << "Bow attack: Shooting an arrow in direction: " << direction.x << ", " << direction.y << std::endl;
        increaseConsecutiveAttacks();
        
        // Crear una flecha y añadirla al contenedor
        arrows.emplace_back(position, direction, arrowSpeed);
        float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
        SFXManager::getInstance().addEffect("resources/sfx/bow.wav", 60.f, pitch);

        if (quickShotEnabled) {
            float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            if (randomValue < quickShotChance) {
              // std::cout << "Quick Shot triggered! Firing a second arrow!" << std::endl;
                // Small delay for the second arrow to simulate rapid fire
                sf::Vector2f slightOffset = direction * 20.f;
                arrows.emplace_back(position + slightOffset, direction, arrowSpeed * 1.1f);
                // Higher pitch for the second arrow
                pitch = 1.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.3f - 1.0f);
                SFXManager::getInstance().addEffect("resources/sfx/bow.wav", 65.f, pitch);
            }
        }

        // Reiniciar el cooldown
        attackTimer = attackCooldown;
    } else {
      // std::cout << "Attack on cooldown! Time remaining: " << attackTimer << " seconds" << std::endl;
    }
}


void Bow::useAbility(sf::Vector2f position, sf::Vector2f direction) {
    if (abilityTimer <= 0.f) { // Verificar si el cooldown ha terminado
      // std::cout << "Bow ability: Shooting multiple arrows in an arc!" << std::endl;

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
      // std::cout << "Ability on cooldown! Time remaining: " << abilityTimer << " seconds" << std::endl;
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
void Bow::renderOnPlayer(sf::Vector2f position, sf::Vector2f direction,sf::RenderWindow& window) {
    // Normalize the direction vector
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Calculate angle in degrees
    float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
    
    // Set the origin to the center of the bow sprite for proper rotation
    // Assuming spriteFacade has a getTexture() method to get dimensions
    sf::Vector2u textureSize = {48, 48}; // Placeholder for actual texture size
    spriteFacade.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
    
    // Distance from player to bow
    float bowDistance = 30.f;
    
    // Calculate new position
    sf::Vector2f bowPosition = position + direction * bowDistance;
    
    // Position the bow
    spriteFacade.setPosition(bowPosition.x, bowPosition.y);
    spriteFacade.setRotation(angle + 45);
    
    render(window);
}

/**
 * Dibuja el arco en la posicion y con la direccion que tiene su sprite por defecto.
 */
void Bow::render(sf::RenderWindow& window){
    // Dibujar el sprite del arma
    spriteFacade.draw(engine->getWindow());
    for(const Arrow& arrow : arrows) {
        arrow.draw(engine->getWindow()); // Dibujar cada flecha
    }
}

void Bow::increaseAttackDamage(float damage) {
    baseDamage += damage; // Aumentar el daño base del arco
}

void Bow::increaseAbilityArrowCount(int count) {
    abilityArrowCount += count; // Incrementar el número de flechas de la habilidad
}

void Bow::increaseArrowSpeed(float speed) {
    arrowSpeed += speed; // Incrementar la velocidad de las flechas
}

void Bow::enableQuickShot(float chance) {
    quickShotEnabled = true;
    quickShotChance += chance; // Allow the chance to stack if multiple items are collected
    if (quickShotChance > 0.75f) quickShotChance = 0.75f; // Cap at 75% to prevent it from becoming too powerful
  // std::cout << "Quick Shot enabled with " << quickShotChance * 100 << "% chance!" << std::endl;
}

void Bow::disableQuickShot() {
    quickShotEnabled = false;
    quickShotChance = 0.0f; // Reset the chance
  // std::cout << "Quick Shot disabled!" << std::endl;
}

void Bow::decreaseAbilityCooldown(float cooldown) {
    abilityCooldown -= cooldown; // Disminuir el cooldown de la habilidad
    if (abilityCooldown < 1.0f) abilityCooldown = 1.0f; // Cap at 1 second to prevent it from becoming too powerful
}