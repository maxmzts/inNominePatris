#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "game_class/Game.h"
#include "include/config.h"

int main() {

    //Inicializar motor de juego
    Game game;
  
    //Bucle del juego
    while (game.running()) {

        //actualizar
        game.update();
        
        //renderizar
        game.render();
    }

  return 0;
}