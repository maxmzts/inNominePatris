#include "Sword.h"
#include <iostream>
#include "Weapon.h"
#include "Character.h"
#include "Enemy.h"

Sword::Sword() : MeleeWeapon(50.0f) {}

void Sword::attack(Character& character, std::vector<Enemy>& enemies) {
    std::cout << "Sword attack!" << std::endl;

    //Obtiene la posicion del personaje
    sf::Vector2f position = character.getPosition();

    //Define una hitbox del ataque delante del personaje
    sf::FloatRect hitbox = sf::FloatRect(position.x, position.y, attackRange, attackRange);

    //Verifica si hay colision con enemigos
    for(Enemy& enemy : enemies) {
        if(hitbox.intersects(enemy.getBounds())) {
            enemy.takeDamage(10);
        }
    }
}

void Sword::useAbility(Character& character) {
    std::cout << "Sword ability!" << std::endl;
    character.startDash(300.0f, 0.5f);
    //Aqui llama al metodo de la clase character para que haga el dash
}