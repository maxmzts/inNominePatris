#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Character.h"
#include "Sword.h"
#include "Enemy.h"

#define kVel 5

int main() {
    // Creamos una ventana
    sf::RenderWindow window(sf::VideoMode(640, 480), "Prueba de Clases");

    // Cargamos la textura del personaje
    sf::Texture tex;
    if (!tex.loadFromFile("resources/sprites.png")) {
        std::cerr << "Error cargando la imagen sprites.png";
        return 1;
    }

    // Cargamos la textura de la espada
    sf::Texture swordTex;
    if (!swordTex.loadFromFile("resources/sword.png")) {
        std::cerr << "Error cargando la imagen sword.png";
        return 1;
    }

    // Creamos el personaje y le asignamos la textura
    Character player;
    player.setTexture(tex);
    player.setOrigin(75 / 2, 75 / 2);
    player.setTextureRect(0 * 75, 0 * 75, 75, 75);
    player.setPosition(240, 320);

    // Creamos la espada y la equipamos al personaje
    Sword sword;
    player.equipWeapon(&sword);

    // Creamos enemigos de prueba
    std::vector<Enemy> enemies = { Enemy(160, 320), Enemy(450, 320) };

    for (auto& enemy : enemies) {
        enemy.setTexture(tex);
        enemy.setTextureRect(0 * 75, 0 * 75, 75, 75);
    }    

    // Reloj para el control de tiempo
    sf::Clock clock;

    // Bucle del juego
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Detectar ataque solo cuando se presiona el botón del ratón
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    player.attack(enemies); // Ataque básico
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    player.useAbility(); // Habilidad especial
                }
            }

            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                
                switch (event.key.code) {
                case sf::Keyboard::Right:
                    player.setTextureRect(0 * 75, 2 * 75, 75, 75);
                    player.setScale(1, 1);
                    player.move(kVel, 0);
                    player.setDirection(1.0f, 0.0f);
                    break;
                case sf::Keyboard::Left:    
                    player.setTextureRect(0 * 75, 2 * 75, 75, 75);
                    player.setScale(-1, 1);
                    player.move(-kVel, 0);
                    player.setDirection(-1.0f, 0.0f);
                    break;
                case sf::Keyboard::Up:
                    player.setTextureRect(0 * 75, 3 * 75, 75, 75);
                    player.move(0, -kVel);
                    player.setDirection(0.0f, -1.0f);
                    break;
                case sf::Keyboard::Down:
                    player.setTextureRect(0 * 75, 0 * 75, 75, 75);
                    player.move(0, kVel);
                    player.setDirection(0.0f, 1.0f);
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;

                    //Cualquier tecla desconocida se imprime por pantalla su código
                default:
                    std::cout << event.key.code << std::endl;
                    break;
                }
            }
        }

        // Actualizar lógica del personaje y enemigos
        float deltaTime = clock.restart().asSeconds();
        player.update(deltaTime);

        // Dibujado de la escena
        window.clear();
        player.draw(window);
        for (Enemy& enemy : enemies) {
            enemy.draw(window);
        }
        window.display();
    }

    return 0;
}
