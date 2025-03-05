#include <SFML/Graphics.hpp>
#include <iostream>

#include "include/config.h"
#include "ej_modulos/mimodulo.h"
#include "character/Character.h"

#define kVel 5

int main() {

  // MiModulo *mod = new MiModulo();

  //Creamos una ventana
  sf::RenderWindow window(sf::VideoMode(640, 480), "P0. Fundamentos de los Videojuegos. DCCIA");

  //Creamos el personaje
  Character character("resources/sprites.png");

  //Bucle del juego
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else {
                character.handleInput(event);
            }
        }
    }

    // Actualiza la lógica del personaje
    character.update();

    window.clear();
    character.draw(window);
    window.display();
}


  return 0;
}
