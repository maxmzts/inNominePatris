#include "PauseMenu.h"
#include "ConfMenu.h"
#include "MainMenu.h"
#include "InGame.h"
#include "../Game.h"
#include <iostream>

PauseMenu* PauseMenu::instance = nullptr;

PauseMenu::PauseMenu(float width, float height) : selectedItemIndex(0) {
    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente. Usando texto sin fuente" << std::endl;
    }

    // Cargar la imagen de fondo
    if (!backgroundTexture.loadFromFile("./resources/background.png")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Escalar el fondo para que ocupe toda la ventana
    sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(width / textureSize.x, height / textureSize.y);
    backgroundSprite.setPosition(0, 0); // Asegurarse de que esté centrado

    // Opciones del menú
    std::vector<std::string> opciones = {"Continuar", "Configuracion", "Salir"};

    for (size_t i = 0; i < opciones.size(); ++i) {
        // Crear fondo del texto
        sf::RectangleShape background(sf::Vector2f(300, 50));
        float spacing = height / (opciones.size() + 1);
        background.setPosition((width - 300) / 2, spacing * (i + 1));
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
        text.setPosition(background.getPosition().x + background.getSize().x / 2,
                         background.getPosition().y + background.getSize().y / 2);

        menuItems.push_back(text);
    }
}
PauseMenu* PauseMenu::getInstance(float width, float height) {
    if (!instance) {
        instance = new PauseMenu(width, height);
    }
    return instance;
}

void PauseMenu::update(Game& game) {
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
                    // Continuar
                    game.changeState(InGame::getInstance(game.getEngine()));
                    return;
                } else if (selectedItemIndex == 1) {
                    // Configuración
                    game.changeState(ConfMenu::getInstance(800, 600));
                    return;
                } else if (selectedItemIndex == 2) {
                    // Salir
                    window.close();
                }
            }
        }
    }
}

// void PauseMenu::render(Game& game, sf::RenderWindow& window) {
//     // Restablecer la vista predeterminada para usar coordenadas de pantalla
//     sf::View originalView = window.getView();
//     window.setView(window.getDefaultView());

//     // Dibujar el fondo
//     window.draw(backgroundSprite);

//     // Dibujar las opciones del menú
//     for (size_t i = 0; i < menuBackgrounds.size(); ++i) {
//         window.draw(menuBackgrounds[i]);
//         window.draw(menuItems[i]);
//     }

//     // Restaurar la vista original
//     window.setView(originalView);
// }

void PauseMenu::render(Game& game, sf::RenderWindow& window) {
    // Restablecer la vista predeterminada directamente
    window.setView(window.getDefaultView());

    // Dibujar el fondo
    window.draw(backgroundSprite);

    // Dibujar las opciones del menú
    for (size_t i = 0; i < menuBackgrounds.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuItems[i]);
    }
}

void PauseMenu::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void PauseMenu::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}