#include "Bow.h"
#include "Arrow.h"
#include "Character.h"
#include "Enemy.h"
#include <iostream>
#include <cmath>

Bow::Bow(GameEngine* engine) : Weapon(engine), arrowSpeed(500.0f), abilityArrowCount(5), abilitySpreadAngle(45.0f) {
    spriteFacade.loadTexture("../resources/Weapons/Bow.png");

}

void Bow::attack(Character& character, std::vector<Enemy>& enemies) {
    std::cout << "Bow attack: Shooting an arrow!" << std::endl;

    // Obtener la posición y dirección del personaje
    sf::Vector2f position = character.getPosition();
    sf::Vector2f direction = character.getDirection();

    // Crear una flecha y añadirla al contenedor
    arrows.emplace_back(position, direction, arrowSpeed);
}

void Bow::useAbility(Character& character, std::vector<Enemy>& enemies) {
    std::cout << "Bow ability: Shooting multiple arrows in an arc!" << std::endl;

    // Obtener la posición del personaje
    sf::Vector2f position = character.getPosition();
    sf::Vector2f direction = character.getDirection();

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
}

void Bow::update(float deltaTime, std::vector<Enemy>& enemies) {
    // Actualizar todas las flechas activas
    for (auto& arrow : arrows) {
        arrow.update(deltaTime);
    }

    // Verificar colisiones con enemigos
    for (auto& arrow : arrows ) {
        for (auto& enemy : enemies) {
            if (arrow.getBounds().intersects(enemy.getBounds())) {
                std::cout << "Enemy hit by arrow!" << std::endl;
                enemy.takeDamage(15); // Daño de la flecha
                arrow.markforRemoval(); // Marcar la flecha para ser eliminada
                break;
            }
        }
    }

    // Eliminar flechas que salgan de la pantalla o estén marcadas para eliminación
    arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
        [](const Arrow& arrow) {
            return arrow.isMarkedForRemoval() ||
                   arrow.getBounds().left > 640 || arrow.getBounds().top > 480 ||
                   arrow.getBounds().left + arrow.getBounds().width < 0 ||
                   arrow.getBounds().top + arrow.getBounds().height < 0;
        }),
        arrows.end());
}

void Bow::draw(GameEngine& engine, const Character* character) {
    if (character) {
        // El arma está equipada, dibujar dependiendo del personaje
        sf::Vector2f position = character->getPosition();
        sf::Vector2f direction = character->getDirection();

        // Ajustar la posición del arma en función de la dirección
        if (direction.x > 0) {  // Mirando a la derecha
            spriteFacade.setPosition(position.x + 20, position.y);
            spriteFacade.setRotation(0); // Sin rotación
        } else if (direction.x < 0) {  // Mirando a la izquierda
            spriteFacade.setPosition(position.x - 20, position.y);
            spriteFacade.setRotation(180); // Rotar 180 grados
        } else if (direction.y < 0) {  // Mirando hacia arriba
            spriteFacade.setPosition(position.x, position.y - 20);
            spriteFacade.setRotation(270); // Rotar 270 grados
        } else if (direction.y > 0) {  // Mirando hacia abajo
            spriteFacade.setPosition(position.x, position.y + 20);
            spriteFacade.setRotation(90); // Rotar 90 grados
        }
    }

    // Dibujar el sprite del arma
    spriteFacade.draw(engine.getWindow());
    for(const Arrow& arrow : arrows) {
        arrow.draw(engine.getWindow()); // Dibujar cada flecha
    }
}

void Bow::increaseAbilityArrowCount(int count) {
    abilityArrowCount += count; // Incrementar el número de flechas de la habilidad
}

void Bow::increaseArrowSpeed(float speed) {
    arrowSpeed += speed; // Incrementar la velocidad de las flechas
}