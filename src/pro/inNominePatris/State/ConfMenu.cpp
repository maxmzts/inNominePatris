#include "ConfMenu.h"
#include "MainMenu.h"
#include "../Game.h"
#include <iostream>

ConfMenu* ConfMenu::instance = nullptr;

ConfMenu::ConfMenu(float width, float height) : selectedItemIndex(0) {
    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente. Usando texto sin fuente" << std::endl;
    }

    // Cargar la imagen de fondo
    if (!backgroundTexture.loadFromFile("./resources/background.png")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(width / backgroundTexture.getSize().x, height / backgroundTexture.getSize().y);

    // Opciones del menú
    std::vector<std::string> opciones = {"Graficos", "Accesibilidad", "Sonido", "Menu Principal"};

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
}

ConfMenu* ConfMenu::getInstance(float width, float height) {
    if (!instance) {
        instance = new ConfMenu(width, height);
    }
    return instance;
}

void ConfMenu::update(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::W) {
                moveUp();
            } else if (event.key.code == sf::Keyboard::S) {
                moveDown();
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (selectedItemIndex == 0) {
                    std::cout << "Graficos seleccionados\n";
                } else if (selectedItemIndex == 1) {
                    std::cout << "Accesibilidad seleccionada\n";
                } else if (selectedItemIndex == 2) {
                    std::cout << "Sonido seleccionado\n";
                } else if (selectedItemIndex == 3) {
                    std::cout << "Menu principal seleccionado\n";
                    // game.changeState(MainMenu::getInstance(GameEngine engine, 800, 600));
                }
            }
        }
    }
}

void ConfMenu::render(Game& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);

    for (size_t i = 0; i < menuItems.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuItems[i]);
    }
}

void ConfMenu::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void ConfMenu::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}