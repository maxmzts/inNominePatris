#include "PauseMenu.h"
#include "ControlsMenu.h" 
#include "MainMenu.h"
#include "InGame.h"
#include "../Game.h"
#include <iostream>

PauseMenu* PauseMenu::instance = nullptr;

PauseMenu::PauseMenu(float width, float height) : selectedItemIndex(0) {
    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/PIXEL.ttf")) {
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
    std::vector<std::string> opciones = {"Continuar", "Controles", "Salir"}; // Changed "Configuracion" to "Controles"

    for (size_t i = 0; i < opciones.size(); ++i) {
        float spacing = height / (opciones.size() + 1);

        // Crear texto del menú
        sf::Text text;
        text.setFont(font);
        text.setString(opciones[i]);
        text.setFillColor(i == 0 ? sf::Color(194, 199, 162) : sf::Color::White);
        defaultButtonTextColor = text.getFillColor();
        text.setCharacterSize(30);

        // Centrar el texto dentro del recuadro
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition((width - textBounds.width) / 2, spacing * (i + 1));

        // Initialize the underline
        sf::RectangleShape underline(sf::Vector2f(textBounds.width, 2)); // Set the line's thickness
        underline.setFillColor(sf::Color(194, 199, 162)); // Set the line's color to #c2c7a2
        underline.setPosition(text.getPosition().x, text.getPosition().y + textBounds.height + 8); // Position under the text with a small offset
        underlines.push_back(underline);

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
                    // Controles
                    ControlsMenu* controlsMenu = ControlsMenu::getInstance(game.getEngine(), 800, 600);
                    game.changeState(controlsMenu);
                    return;
                }  else if (selectedItemIndex == 2) {
                    // Salir
                    window.close();
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleMouseClick(game, window);
            }
        }
    }
}

void PauseMenu::handleMouseClick(Game& game, sf::RenderWindow& window) {
    // Obtener la posición del ratón en coordenadas de la ventana
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Verificar si el ratón está sobre alguna opción del menú
    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (menuItems[i].getGlobalBounds().contains(worldPos)) {
            selectedItemIndex = i; // Actualizar la opción seleccionada
            handleSelection(game); // Ejecutar la acción correspondiente
            break;
        }
    }
}

void PauseMenu::handleSelection(Game& game) {
    if (selectedItemIndex == 0) {
        // Continuar
        game.changeState(InGame::getInstance(game.getEngine()));
    } else if (selectedItemIndex == 1) {
        // Controles
        ControlsMenu* controlsMenu = ControlsMenu::getInstance(game.getEngine(), 800, 600);
        //controlsMenu->setPreviousState(this, this); REMOVE THIS LINE
        game.changeState(controlsMenu);
        return;
    } else if (selectedItemIndex == 2) {
        // Salir
        game.getWindow().close();
    }
}

void PauseMenu::render(Game& game, sf::RenderWindow& window) {
    // Restablecer la vista predeterminada directamente
    window.setView(window.getDefaultView());

    // Dibujar el fondo
    window.draw(backgroundSprite);

    // Dibujar las opciones del menú
    for (size_t i = 0; i < menuItems.size(); ++i) {
        window.draw(menuItems[i]);
         if (i == selectedItemIndex) {
           window.draw(underlines[i]);
        }
    }
}

void PauseMenu::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color(194, 199, 162)); // Set the text color to #c2c7a2
    }
}

void PauseMenu::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color(194, 199, 162)); // Set the text color to #c2c7a2
    }
}