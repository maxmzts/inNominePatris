#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"

#include "ej_modulos/mimodulo.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu Principal");
    Menu menu(800, 600);
    MiModulo mod;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W)
                    menu.moveUp();
                else if (event.key.code == sf::Keyboard::S)
                    menu.moveDown();
                else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = menu.getSelectedItem();
                    if (selected == 0) {
                        std::cout << "Nueva Partida seleccionada\n";
                        window.close();
                        mod.iniciarNuevaPartida();
                    } else if (selected == 1) {
                        std::cout << "Continuar seleccionada\n";
                    } else if (selected == 2) {
                        std::cout << "Salir seleccionado\n";
                        window.close();
                    }
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}
