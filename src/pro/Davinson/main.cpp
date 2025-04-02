#include <SFML/Graphics.hpp>
#include <iostream>

#include "include/config.h"
#include "ej_modulos/mimodulo.h"
#include "character/Character.h"
#include "TileMap.h"
#include "TileSet.h"




#define kVel 5

int main() {

  // MiModulo *mod = new MiModulo();

  //Creamos una ventana
  sf::RenderWindow window(sf::VideoMode(640, 480), "P0. Fundamentos de los Videojuegos. DCCIA");






  // Configurar la vista (cámara)
  sf::View view(sf::FloatRect(0, 0, 640, 480));




  //Creamos el personaje
  Character character("resources/sprites.png");




   // Crear el TileMap y cargar el mapa
   TileMap tileMap;
if (!tileMap.loadFromFile("maps/lobby.tmx")) {
    return -1;
}




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

    // Mover la cámara para que siga al personaje
    view.setCenter(character.getPosition());
    window.setView(view);


    window.clear();
    window.draw(tileMap); // Dibujar el mapa


    character.draw(window);
    window.display();
}


  return 0;
}
