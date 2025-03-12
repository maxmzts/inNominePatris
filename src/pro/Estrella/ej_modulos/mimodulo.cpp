#include "mimodulo.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Interface/HUD.h"

#define kVel 5

MiModulo::MiModulo() {
    std::cout << "Creando mi modulo..."; 
}

void MiModulo::iniciarNuevaPartida() {
    std::cout << "Iniciando nueva partida..." << std::endl;
    MiModulo *mod = new MiModulo();

    // Creamos una ventana
    sf::RenderWindow window(sf::VideoMode(640, 480), "P0. Fundamentos de los Videojuegos. DCCIA");

    // Cargo la imagen donde reside la textura del sprite
    sf::Texture tex;
    if (!tex.loadFromFile("../resources/sprites.png")) {
        std::cerr << "Error cargando la imagen sprites.png";
        exit(0);
    }

    // Creo el spritesheet a partir de la imagen anterior
    sf::Sprite sprite(tex);

    // Le pongo el centroide donde corresponde
    sprite.setOrigin(75 / 2, 75 / 2);
    // Cojo el sprite que me interesa por defecto del sheet
    sprite.setTextureRect(sf::IntRect(0 * 75, 0 * 75, 75, 75));

    // Lo dispongo en el centro de la pantalla
    sprite.setPosition(320, 240);

    // Crear el HUD
    HUD hud(window.getSize().x, window.getSize().y);

    // Variables de salud y cooldown
    float playerHealth = 100.0f; // Salud inicial del jugador
    float skillCooldown = 0.0f; // Cooldown inicial

    // Bucle del juego
    while (window.isOpen()) {
        // Bucle de obtención de eventos
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Right:
                    sprite.setTextureRect(sf::IntRect(0 * 75, 2 * 75, 75, 75));
                    sprite.setScale(1, 1);
                    sprite.move(kVel, 0);
                    break;
                case sf::Keyboard::Left:
                    sprite.setTextureRect(sf::IntRect(0 * 75, 2 * 75, 75, 75));
                    sprite.setScale(-1, 1);
                    sprite.move(-kVel, 0);
                    break;
                case sf::Keyboard::Up:
                    sprite.setTextureRect(sf::IntRect(0 * 75, 3 * 75, 75, 75));
                    sprite.move(0, -kVel);
                    break;
                case sf::Keyboard::Down:
                    sprite.setTextureRect(sf::IntRect(0 * 75, 0 * 75, 75, 75));
                    sprite.move(0, kVel);
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    std::cout << event.key.code << std::endl;
                    break;
                }
            }
        }

        // Actualizar el HUD (simulando que el jugador pierde salud y el cooldown avanza)
        if(playerHealth > 0) {
            playerHealth -= 0.1f; // Reducimos la salud por cada frame
        } else if(playerHealth == 0) {
            std::cout << "The end" << std::endl;
            window.close();
        }
        skillCooldown += 0.05f; // Aumentamos el cooldown por cada frame

        if (skillCooldown > 10.0f) skillCooldown = 10.0f; // Limitamos el cooldown a un máximo

        // Actualizamos las barras en el HUD
        hud.update(playerHealth, skillCooldown);

        // Dibujar todo
        window.clear();
        window.draw(sprite);
        hud.draw(window, sprite); // Dibujar el HUD sobre el jugador
        window.display();
    }

    delete mod;
}
