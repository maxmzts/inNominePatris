#include "GameEngine.h"
#include "TileMap.h"
#include "Character.h"
#include "Weapon/Sword.h"
#include "Weapon/Lance.h"
#include "Weapon/Bow.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm> // Para std::remove_if

int main() {
    // Crear el motor del juego
    GameEngine engine("In Nomine Patris", 800, 600);

    // Cargar el mapa
    TileMap tileMap;
    if (!tileMap.loadFromFile("maps/world_1.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        return -1;
    }

    // Crear el personaje principal
    Character player("resources/sprites.png");
    
    // Hacer spawn al jugador en una posición específica (lo emplearemos para cambiar de salas)
    player.spawnAt(tileMap, 500, 600);


    // Crear las armas y colocarlas en los pilares
    Sword* sword = new Sword(&engine);
    Lance* lance = new Lance(&engine);
    Bow* bow = new Bow(&engine);

    // Lista de armas en el suelo
    std::vector<Weapon*> weaponsOnGround = { sword, lance, bow };

    // Posiciones de los pilares (IDs de interacción: 1375, 1376, 1440)
    sf::Vector2f swordPosition(183, 530); // Ajusta según la posición del pilar en el mapa
    sf::Vector2f lancePosition(234, 500);
    sf::Vector2f bowPosition(163, 578);

    sword->setPosition(swordPosition.x, swordPosition.y);
    lance->setPosition(lancePosition.x, lancePosition.y);
    bow->setPosition(bowPosition.x, bowPosition.y);

    sf::Clock clock;
    sf::RenderWindow& window = engine.getWindow();
    
    while (engine.isRunning()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                player.handleInput(event);
            }

            // Tecla para hacer respawn en una posición diferente (para pruebas)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                // Coordenadas de spawn aleatorias para pruebas
                float randomX = rand() % 500;
                float randomY = rand() % 500;
                player.spawnAt(tileMap, randomX, randomY);
            }

            // // Interacción con los pilares
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
                sf::Vector2f playerPosition = player.getPosition();
            
                // Verificar si el jugador está cerca de un arma en el suelo
                auto it = std::find_if(weaponsOnGround.begin(), weaponsOnGround.end(), [&](Weapon* weapon) {
                    sf::Vector2f weaponPosition = weapon->getPosition();
                    return std::abs(playerPosition.x - weaponPosition.x) < 50 &&
                           std::abs(playerPosition.y - weaponPosition.y) < 50;
                });
            
                if (it != weaponsOnGround.end()) {
                    Weapon* weapon = *it;
                    sf::Vector2f weaponPos = weapon->getPosition();
                    
                    if (player.getWeaponCount() < 2) {
                        // Player has 0 or 1 weapons, just add the new one
                        player.addWeaponWithPosition(weapon, weaponPos);
                        weaponsOnGround.erase(it); // Remove from ground
                        player.equipWeapon();
                        std::cout << "Arma equipada!" << std::endl;
                    } else {
                        // Player already has 2 weapons, replace the first one
                        sf::Vector2f oldWeaponPos;
                        Weapon* oldWeapon = player.removeFirstWeapon(oldWeaponPos);
                        
                        // Add the new weapon
                        player.addWeaponWithPosition(weapon, weaponPos);
                        weaponsOnGround.erase(it); // Remove from ground
                        
                        // Return old weapon to ground
                        oldWeapon->setPosition(oldWeaponPos.x, oldWeaponPos.y);
                        weaponsOnGround.push_back(oldWeapon);
                        
                        player.equipWeapon();
                        std::cout << "Arma reemplazada y equipada!" << std::endl;
                    }
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
                    sf::Vector2f playerPosition = player.getPosition();
                    
                    // Primero comprobar interacción con tiles
                    player.interact(tileMap);
                    
                    // Verificar si el jugador está cerca de un arma en el suelo
                    auto it = std::find_if(weaponsOnGround.begin(), weaponsOnGround.end(), [&](Weapon* weapon) {
                        sf::Vector2f weaponPosition = weapon->getPosition();
                        return std::abs(playerPosition.x - weaponPosition.x) < 50 &&
                               std::abs(playerPosition.y - weaponPosition.y) < 50;
                    });
                
                    if (it != weaponsOnGround.end()) {
                        Weapon* weapon = *it;
                        sf::Vector2f weaponPos = weapon->getPosition();
                        
                        if (player.getWeaponCount() < 2) {
                            // Player has 0 or 1 weapons, just add the new one
                            player.addWeaponWithPosition(weapon, weaponPos);
                            weaponsOnGround.erase(it); // Remove from ground
                            player.equipWeapon();
                            std::cout << "Arma equipada!" << std::endl;
                        } else {
                            // Player already has 2 weapons, replace the first one
                            sf::Vector2f oldWeaponPos;
                            Weapon* oldWeapon = player.removeFirstWeapon(oldWeaponPos);
                            
                            // Add the new weapon
                            player.addWeaponWithPosition(weapon, weaponPos);
                            weaponsOnGround.erase(it); // Remove from ground
                            
                            // Return old weapon to ground
                            oldWeapon->setPosition(oldWeaponPos.x, oldWeaponPos.y);
                            weaponsOnGround.push_back(oldWeapon);
                            
                            player.equipWeapon();
                            std::cout << "Arma reemplazada y equipada!" << std::endl;
                        }
                    }
                }
            }

            // Cambiar de arma
            if(player.getEquippedWeapon() != nullptr) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                    player.switchWeapon();
                    player.equipWeapon();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Ataque normal
                    std::vector<Enemy> enemies; // Deberías tener una lista real de enemigos
                    player.attack(enemies);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    // Usar habilidad especial
                    std::vector<Enemy> enemies; // Deberías tener una lista real de enemigos
                    player.useAbility(window, enemies);
                }
            }
        }

        // Actualizar el personaje y otros elementos del juego
        float deltaTime = clock.restart().asSeconds();
        player.update(tileMap, deltaTime);
        
        std::vector<Enemy> enemies;
        if (bow) { // Verificar que el arco exista antes de usarlo
            bow->update(deltaTime, enemies); // Actualiza las flechas
        }
        
        // Actualizar la posición de la cámara para seguir al jugador
        engine.setViewCenter(player.getPosition());

        // Dibujar todo en la ventana
        engine.clear();
        tileMap.draw(engine);

        // Dibujar las armas en el suelo
        for (Weapon* weapon : weaponsOnGround) {
            weapon->draw(engine, nullptr);
        }

        // Dibujar el personaje
        player.draw(engine);

        engine.display();
    }

    // Liberar memoria de las armas
    delete sword;
    delete lance;
    delete bow;

    return 0;
}