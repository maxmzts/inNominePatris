#include "MainMenu.h"
#include "InGame.h"
#include "ConfMenu.h"
#include "../Game.h"
#include <iostream>

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(GameEngine& engine, float width, float height) :engine(engine), selectedItemIndex(0) {
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

    // Opciones del menú
    std::vector<std::string> opciones = {"Nueva Partida", "Continuar partida", "Configuracion", "Salir"};

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
        text.setPosition(background.getPosition().x + 300 / 2, background.getPosition().y + 60 / 2 - 5);

        menuItems.push_back(text);
    }
}

MainMenu* MainMenu::getInstance(GameEngine& engine, float width, float height) {
    if (!instance) {
        instance = new MainMenu(engine, width, height);
    }
    return instance;
}

void MainMenu::update(Game& game) {
    sf::RenderWindow& window = game.getWindow();

    // Manejar eventos
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
                // Acción según la opción seleccionada
                if (selectedItemIndex == 0) {
                    std::cout << "Nueva Partida seleccionada\n";
                    game.changeState(InGame::getInstance(engine));
                } else if (selectedItemIndex == 1) {
                    std::cout << "Continuar partida seleccionada\n";
                    game.changeState(InGame::getInstance(engine));
                } else if (selectedItemIndex == 2) {
                    std::cout << "Configuración seleccionada\n";
                    game.changeState(ConfMenu::getInstance(800, 600));
                } else if (selectedItemIndex == 3) {
                    std::cout << "Salir seleccionado\n";
                    window.close();
                }
            }
        }
    }
}

void MainMenu::render(Game& game, sf::RenderWindow& window) {
    window.draw(backgroundSprite);

    for (size_t i = 0; i < menuItems.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuItems[i]);
    }
}

void MainMenu::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void MainMenu::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}