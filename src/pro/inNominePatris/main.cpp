#include "GameEngine.h"
#include "TileMap.h"
#include "Character.h"
#include "Weapon/Sword.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>


int main() {
    // Crear el motor del juego
    GameEngine engine("In Nomine Patris", 800, 600);

    // Cargar el mapa
    TileMap tileMap;
    if (!tileMap.loadFromFile("maps/lobby.tmx", engine)) {
        std::cerr << "Error cargando el mapa\n";
        return -1;
    }

    // Crear el personaje principal
    Character player("resources/sprites.png");

    // Crear una espada y equipársela al personaje
    Sword* sword = new Sword(&engine); // Crear la espada
    player.addWeapon(sword);           // Añadir la espada al personaje
    player.equipWeapon();              // Equipar la espada

    // Ciclo principal del juego
    while (engine.isRunning()) {
        sf::Event event;
        sf::RenderWindow& window = engine.getWindow();
        
        // Manejar eventos
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else {
                    player.handleInput(event);
                }
            }

            // Manejo de eventos de ataque y habilidades
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

            // Cambiar de arma
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                player.switchWeapon();
                player.equipWeapon();
            }
        }

        // Actualizar el personaje y otros elementos del juego
        sf::Clock clock;
        float deltaTime = clock.restart().asSeconds();
        player.update(tileMap, deltaTime);

        // Actualizar la posición de la cámara para seguir al jugador
        engine.setViewCenter(player.getPosition());

        // Dibujar todo en la ventana
        engine.clear();
        tileMap.draw(engine);
        player.draw(engine);
        engine.display();
    }

    // Liberar memoria de la espada
    delete sword;

    return 0;
}