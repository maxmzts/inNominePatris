#include "Sword.h"
#include <iostream>
#include "Weapon.h"
#include "Character.h"
#include "Enemy.h"

Sword::Sword() : attackRange(50.0f), dashSpeed(500.0f), abilityCooldown(1.5f), lastAbilityTime(-abilityCooldown){}

void Sword::attack(sf::Vector2f position, sf::Vector2f direction std::vector<Enemy>& enemies) {
    std::cout << "Sword attack!" << std::endl;

    

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
    static sf::Clock clock;
    float elapsedTime = clock.getElapsedTime().asSeconds();
    if(elapsedTime - lastAbilityTime < abilityCooldown) {
        std::cout << "Ability on cooldown!" << std::endl;
        return;
    }

    lastAbilityTime = elapsedTime;
    std::cout << "Sword ability!" << std::endl;
    character.startDash(dashSpeed, 0.2f);
    //Aqui llama al metodo de la clase character para que haga el dash
}

void Sword::increaseDashSpeed(float speed) {
    std::cout << "Dash speed increased!" << std::endl;
    dashSpeed += speed;
}

void Sword::increaseAttackRange(float range) {
    std::cout << "Attack range increased!" << std::endl;
    attackRange += range;
}