#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Character.h"
#include "Sword.h"
#include "Enemy.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ataque en Arco");

    // Cargar textura del personaje
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("resources/sprites.png")) {
        std::cerr << "Error: No se pudo cargar la textura del personaje." << std::endl;
        return -1;
    }

    // Crear el personaje y asignarle la textura
    Character player;
    // Cargar la hoja de sprites
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("resources/sprites.png")) {
        std::cerr << "Error cargando la imagen sprites.png" << std::endl;
        return -1;
    }
    player.setTexture(playerTexture);

    // Establecer la posición inicial
    player.setTextureRect(0 * 75, 0 * 75, 75, 75); // Sprite inicial

    Sword sword;
    player.equipWeapon(&sword);

    std::vector<Enemy> enemies = {Enemy(400, 300), Enemy(450, 320)};
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        while (window.pollEvent(event)) {  
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    player.attack(enemies); // Solo se ejecuta una vez por clic
                }
            }
        }   

        if(sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Right)
                player.useAbility(); // Habilidad especial
        }

        float deltaTime = clock.restart().asSeconds();
        player.update(deltaTime);

        window.clear();
        player.draw(window);
        for (Enemy& enemy : enemies) {
            enemy.draw(window);
        }
        window.display();
    }

    return 0;
}
