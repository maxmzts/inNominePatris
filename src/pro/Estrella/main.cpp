#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"

#include "ej_modulos/mimodulo.h"
#include "MenuConf.h"

void menuConf();

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
                        std::cout << "Configuración seleccionado\n";
                        window.close();
                        menuConf();
                    } else if (selected == 3) {
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

void menuConf() {
    // Crear una nueva ventana para el menú de configuración
    sf::RenderWindow configWindow(sf::VideoMode(800, 600), "Menu Configuracion");
    MenuConf configMenu(800, 600);

    while (configWindow.isOpen()) {
        sf::Event configEvent;
        while (configWindow.pollEvent(configEvent)) {
            if (configEvent.type == sf::Event::Closed)
                configWindow.close();

            if (configEvent.type == sf::Event::KeyPressed) {
                if (configEvent.key.code == sf::Keyboard::W)
                    configMenu.moveUp();
                else if (configEvent.key.code == sf::Keyboard::S)
                    configMenu.moveDown();
                else if (configEvent.key.code == sf::Keyboard::Enter) { // Regresar al menú principal
                    int selected = configMenu.getSelectedItem();
                    if (selected == 0) {
                        std::cout << "Graficos seleccionada\n";
                        configWindow.close();
                    } else if (selected == 1) {
                        std::cout << "Accesibilidad seleccionada\n";
                        configWindow.close();
                    } else if (selected == 2) {
                        std::cout << "Sonido seleccionado\n";
                        configWindow.close();
                    } else if (selected == 3) {
                        std::cout << "Controles seleccionado\n";
                        configWindow.close();
                    }
                }
            }
        }

        configWindow.clear();
        configMenu.draw(configWindow);
        configWindow.display();
    }
}
