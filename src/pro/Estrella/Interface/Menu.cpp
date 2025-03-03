// Menu.cpp
#include "Menu.h"

Menu::Menu(float width, float height) {
    // if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    //     // Manejo de error
    // }

    std::vector<std::string> opciones = {"Nueva Partida", "Continuar", "Salir"};
    for (size_t i = 0; i < opciones.size(); ++i) {
        sf::Text text;
        // text.setFont(font);
        text.setString(opciones[i]);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2.5, height / (opciones.size() + 1) * (i + 1)));
        menuItems.push_back(text);
    }
    selectedItemIndex = 0;
}

void Menu::draw(sf::RenderWindow &window) {
    for (const auto &item : menuItems) {
        window.draw(item);
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
