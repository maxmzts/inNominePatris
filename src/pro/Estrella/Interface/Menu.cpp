#include "Menu.h"
#include <iostream>

Menu::Menu(float width, float height) {
    // Cargar fuente
    if (!font.loadFromFile("assets/fonts/ARIAL.TTF")) {
        std::cerr << "Error al cargar la fuente. Usando texto sin fuente" << std::endl;
    }

    std::vector<std::string> opciones = {"Nueva Partida", "Continuar", "Salir"};
    
    for (size_t i = 0; i < opciones.size(); ++i) {
        // Crear fondo del texto
        sf::RectangleShape background(sf::Vector2f(300, 50));
        background.setPosition(width / 2.5 - 20, height / (opciones.size() + 1) * (i + 1) - 10);
        background.setFillColor(sf::Color(50, 50, 50, 200)); // Color gris con transparencia
        menuBackgrounds.push_back(background);

        // Crear texto del menú
        sf::Text text;
        text.setFont(font);
        text.setString(opciones[i]);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setCharacterSize(30);
        text.setPosition(width / 2.5, height / (opciones.size() + 1) * (i + 1));
        menuItems.push_back(text);
    }

    selectedItemIndex = 0;
}

void Menu::draw(sf::RenderWindow &window) {
    for (size_t i = 0; i < menuItems.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuItems[i]);
    }
}

void Menu::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}
