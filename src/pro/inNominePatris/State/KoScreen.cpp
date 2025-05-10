#include "KoScreen.h"
#include "../Game.h"
#include "MainMenu.h"
#include "InGame.h"
#include <SFML/Graphics.hpp>
#include <iostream>

KoScreen* KoScreen::instance = nullptr;

KoScreen* KoScreen::getInstance() {
    if (!instance) {
        instance = new KoScreen();
    }
    return instance;
}

KoScreen::KoScreen() : selectedItemIndex(0) {
    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error al cargar la fuente para la pantalla de KO\n";
    }

    // Cargar la imagen de fondo
    if (!backgroundTexture.loadFromFile("./resources/background.png")) {
        std::cerr << "Error al cargar la imagen de fondo\n";
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Escalar el fondo para que ocupe toda la ventana
    sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(800.f / textureSize.x, 600.f / textureSize.y);
    backgroundSprite.setPosition(0, 0);

    // Configurar el texto principal
    title.setFont(font);
    title.setString("Has muerto");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Red);

    // Centrar el título en la parte superior
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(400, 150); // Centrado horizontalmente, ajustado verticalmente

    // Configurar las opciones
    std::vector<std::string> opciones = {"Juegar de nuevo", "salir"};
    for (size_t i = 0; i < opciones.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(opciones[i]);
        option.setCharacterSize(30);
        option.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);

        // Posicionar las opciones proporcionalmente
        float spacing = 600.f / (opciones.size() + 2); // Espaciado proporcional
        option.setPosition(400, spacing * (i + 2)); // Centrado horizontalmente
        sf::FloatRect optionBounds = option.getLocalBounds();
        option.setOrigin(optionBounds.width / 2, optionBounds.height / 2);

        menuOptions.push_back(option);
    }
}

void KoScreen::update(Game& game) {
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
                    // Jugar de nuevo
                    game.changeState(InGame::getInstance(game.getEngine()));
                    return;
                } else if (selectedItemIndex == 1) {
                    // Salir
                    window.close();
                }
            }
        }
    }
}

void KoScreen::render(Game& game, sf::RenderWindow& window) {
    window.clear();

    // Dibujar el fondo
    window.draw(backgroundSprite);

    // Dibujar el título
    window.draw(title);

    // Dibujar las opciones del menú
    for (const auto& option : menuOptions) {
        window.draw(option);
    }

    window.display();
}

void KoScreen::moveUp() {
    if (selectedItemIndex > 0) {
        menuOptions[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuOptions[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void KoScreen::moveDown() {
    if (selectedItemIndex < menuOptions.size() - 1) {
        menuOptions[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuOptions[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}