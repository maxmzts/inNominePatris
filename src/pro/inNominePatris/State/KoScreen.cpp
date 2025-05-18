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

    // Escalar el fondo dinámicamente según el tamaño de la ventana
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize(800, 600); // Tamaño predeterminado de la ventana
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    // Configurar el texto principal
    title.setFont(font);
    title.setString("Has muerto");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Red);

    // Centrar el título en la parte superior
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(windowSize.x / 2.f, 100); // Centrado horizontalmente

    // Configurar las opciones
    std::vector<std::string> opciones = {"Jugar de nuevo", "Salir"};
    for (size_t i = 0; i < opciones.size(); ++i) {
        // Crear fondo del texto
        sf::RectangleShape background(sf::Vector2f(300, 50));
        float spacing = windowSize.y / (opciones.size() + 2);
        background.setPosition((windowSize.x - 300) / 2, spacing * (i + 2));
        background.setFillColor(sf::Color(50, 50, 50, 200)); // Color gris con transparencia
        background.setOutlineThickness(2);
        background.setOutlineColor(sf::Color::White);
        menuBackgrounds.push_back(background);

        // Crear texto del menú
        sf::Text option;
        option.setFont(font);
        option.setString(opciones[i]);
        option.setCharacterSize(30);
        option.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);

        // Centrar el texto dentro del recuadro
        sf::FloatRect optionBounds = option.getLocalBounds();
        option.setOrigin(optionBounds.width / 2, optionBounds.height / 2);
        option.setPosition(background.getPosition().x + 300 / 2, background.getPosition().y + 50 / 2);

        menuOptions.push_back(option);
    }
}

void KoScreen::render(Game& game, sf::RenderWindow& window) {
    // Restablecer la vista predeterminada
    window.setView(window.getDefaultView());

    window.clear();

    // Dibujar el fondo
    window.draw(backgroundSprite);

    // Dibujar el título
    window.draw(title);

    // Dibujar las opciones del menú
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuOptions[i]);
    }

    window.display();
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
                handleSelection(game);
                return;
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleMouseClick(game, window);
            }
        }
    }
}

void KoScreen::handleMouseClick(Game& game, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    for (size_t i = 0; i < menuBackgrounds.size(); ++i) {
        if (menuBackgrounds[i].getGlobalBounds().contains(worldPos)) {
            selectedItemIndex = i;
            handleSelection(game);
            break;
        }
    }
}

void KoScreen::handleSelection(Game& game) {
    if (selectedItemIndex == 0) {   
        Character::getInstance()->reset();     
        InGame* inGame = InGame::getInstance(game.getEngine());
        std::string targetWorldState = "lobby", mapFilePath = "./maps/lobby.tmx", musicFilePath = "resources/music/lobby_track.ogg";
        sf::Vector2i spawnPosition = {20, 44};
        inGame->changeWorldState(targetWorldState, mapFilePath, musicFilePath, spawnPosition);
        game.changeState(inGame);
        return;
    } else if (selectedItemIndex == 1) {
        // Salir
        game.getWindow().close();
    }
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