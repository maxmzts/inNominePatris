#include "Sword.h"
#include <iostream>
#include "Weapon.h"
#include "Character.h"
#include "Enemy.h"

Sword::Sword() : MeleeWeapon(100.0f) {}

void Sword::attack(Character& character, std::vector<Enemy>& enemies) {
    std::cout << "Sword attack!" << std::endl;

    // Obtiene la posición y dirección del personaje
    sf::Vector2f position = character.getPosition();
    sf::Vector2f scale = character.getScale(); // Detecta si el personaje mira a la izquierda o derecha

    // Define una hitbox dependiendo de la dirección
    sf::FloatRect hitbox;
    float attackWidth = 50.0f * 1.5f;  // Ancho del ataque
    float attackHeight = 50.0f * 0.5f; // Altura del ataque

    if (scale.x > 0) {  // Mirando a la derecha
        hitbox = sf::FloatRect(position.x + 40, position.y - attackHeight / 2, attackWidth, attackHeight);
    } else if (scale.x < 0) {  // Mirando a la izquierda
        hitbox = sf::FloatRect(position.x - attackWidth - 40, position.y - attackHeight / 2, attackWidth, attackHeight);
    } else {  
        // Si no hay un scale.x definido, se puede manejar de forma alternativa
        hitbox = sf::FloatRect(position.x, position.y, attackRange, attackRange);
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
    character.startDash(1000.0f, 0.2f);
    //Aqui llama al metodo de la clase character para que haga el dash
}