#include "Sword.h"
#include <iostream>
#include "Weapon.h"
#include "Character.h"
#include "Enemy.h"

Sword::Sword() : MeleeWeapon(50.0f), attackRange(50.0f), dashSpeed(500.0f){}

void Sword::attack(Character& character, std::vector<Enemy>& enemies) {
    std::cout << "Sword attack!" << std::endl;

    // Obtiene la posición y dirección del personaje
    sf::Vector2f position = character.getPosition();
    sf::Vector2f direction = character.getDirection(); // Detecta si el personaje mira a la izquierda o derecha

    // Define una hitbox dependiendo de la dirección
    sf::FloatRect hitbox;
    float attackWidth = attackRange * 1.5f;  // Ancho del ataque
    float attackHeight = attackRange * 0.5f; // Altura del ataque

    if (direction.x > 0) {  // Mirando a la derecha
        hitbox = sf::FloatRect(position.x + 40, position.y - attackHeight / 2, attackWidth, attackHeight);
    } else if (direction.x < 0) {  // Mirando a la izquierda
        hitbox = sf::FloatRect(position.x - attackWidth - 40, position.y - attackHeight / 2, attackWidth, attackHeight);
    } else if (direction.y < 0) {  // Mirando hacia arriba
        hitbox = sf::FloatRect(position.x - attackWidth / 2, position.y - attackHeight - 40, attackWidth, attackHeight);
    } else if (direction.y > 0) {  // Mirando hacia abajo
        hitbox = sf::FloatRect(position.x - attackWidth / 2, position.y + 40, attackWidth, attackHeight);
    } else { //hitbox estándar
        hitbox = sf::FloatRect(position.x, position.y, attackWidth, attackHeight);
    }

    // Verifica colisiones con enemigos
    for (Enemy& enemy : enemies) {
        if (hitbox.intersects(enemy.getBounds())) {
            std::cout << "Enemigo golpeado!" << std::endl;
            enemy.takeDamage(10);
        }
    }
}


void Sword::useAbility(Character& character) {
    std::cout << "Sword ability!" << std::endl;
    character.startDash(dashSpeed, 0.2f);
    //Aqui llama al metodo de la clase character para que haga el dash
}

void Sword::increaseDashSpeed(float speed) {
    std::cout << "Dash speed increased!" << std::endl;
    dashSpeed += speed;
}