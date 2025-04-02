#include "MenuConf.h"
#include <iostream>

MenuConf::MenuConf(float width, float height) {
    // Cargar fuente
    if (!font.loadFromFile("../assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente. Usando texto sin fuente" << std::endl;
    }

    // Cargar la imagen de fondo
    if (!backgroundTexture.loadFromFile("../resources/background.png")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(width / backgroundTexture.getSize().x, height / backgroundTexture.getSize().y);

    std::vector<std::string> opciones = {"Graficos", "Accesibilidad", "Sonido", "Controles"};
    
    for (size_t i = 0; i < opciones.size(); ++i) {
        // Crear fondo del texto
        sf::RectangleShape background(sf::Vector2f(300, 50));
        background.setPosition((width - 300) / 2, height / (opciones.size() + 1) * (i + 1));
        background.setFillColor(sf::Color(50, 50, 50, 200)); // Color gris con transparencia
        background.setOutlineThickness(2);
        background.setOutlineColor(sf::Color::White);
        menuBackgrounds.push_back(background);

        // Crear texto del menú
        sf::Text text;
        text.setFont(font);
        text.setString(opciones[i]);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setCharacterSize(30);

        // Centrar el texto dentro del recuadro
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        text.setPosition(background.getPosition().x + 300 / 2, background.getPosition().y + 60 / 2 - 5);
        
        menuItems.push_back(text);
    }

    selectedItemIndex = 0;
}

void MenuConf::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);

    for (size_t i = 0; i < menuItems.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuItems[i]);
    }
}

void MenuConf::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void MenuConf::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}
