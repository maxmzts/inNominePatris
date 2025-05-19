#include "MainMenu.h"
#include "InGame.h"
#include "ControlsMenu.h" // Include the new ControlsMenu
#include "../Game.h"
#include <iostream>
#include <MusicManager.h>

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(GameEngine& engine, float width, float height) :engine(engine), selectedItemIndex(0) {
    MusicManager::getInstance().transitionTo("./resources/music/menu-track.ogg",100.f,1.f,0.01f);
    // Cargar fuente
    if (!font.loadFromFile("./assets/fonts/PIXEL.ttf")) {
        std::cerr << "Error al cargar la fuente. Usando texto sin fuente" << std::endl;
    }

    // Cargar la imagen de fondo
    if (!backgroundTexture.loadFromFile("./resources/background.png")) {
        std::cerr << "Error al cargar la imagen de fondo" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(width / backgroundTexture.getSize().x, height / backgroundTexture.getSize().y);

    // Opciones del menú
    std::vector<std::string> opciones = {"Nueva Partida", "Continuar partida", "Controles", "Salir"}; // Changed "Configuracion" to "Controles"

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

MainMenu* MainMenu::getInstance(GameEngine& engine, float width, float height) {
    if (!instance) {
        instance = new MainMenu(engine, width, height);
    }
    return instance;
}

void MainMenu::update(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    MusicManager::getInstance().update(engine.getDeltaTime());

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
                    std::cout << "Controles seleccionada\n";
                    ControlsMenu* controlsMenu = ControlsMenu::getInstance(engine, 800, 600);
                    game.changeState(controlsMenu);
                } else if (selectedItemIndex == 3) {
                    std::cout << "Salir seleccionado\n";
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

void MainMenu::handleMouseClick(Game& game, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (menuItems[i].getGlobalBounds().contains(worldPos)) {
            selectedItemIndex = i;
            handleSelection(game);
            break;
        }
    }
}

void MainMenu::handleSelection(Game& game) {
    if (selectedItemIndex == 0) {
        std::cout << "Nueva Partida seleccionada\n";
        game.changeState(InGame::getInstance(engine));
    } else if (selectedItemIndex == 1) {
        std::cout << "Continuar partida seleccionada\n";
        game.changeState(InGame::getInstance(engine));
    } else if (selectedItemIndex == 2) {
        std::cout << "Controles seleccionada\n";
        ControlsMenu* controlsMenu = ControlsMenu::getInstance(engine, 800, 600);
        //controlsMenu->setPreviousState(this, this); REMOVE THIS LINE
        game.changeState(controlsMenu);
    } else if (selectedItemIndex == 3) {
        std::cout << "Salir seleccionado\n";
        game.getWindow().close();
    }
}

void MainMenu::render(Game& game, sf::RenderWindow& window) {
    window.clear(); // Limpiar la ventana antes de dibujar
    GameEngine& engine = game.getEngine();

    // Dibujar el fondo
    engine.drawSprite(backgroundSprite);

    // Dibujar las opciones del menú
    for (size_t i = 0; i < menuItems.size(); ++i) {
        engine.drawText(menuItems[i]);
        if (i == selectedItemIndex) {
           window.draw(underlines[i]);
        }
    }
}

void MainMenu::moveUp() {
    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color(194, 199, 162)); // Set the text color to #c2c7a2
    }
}

void MainMenu::moveDown() {
    if (selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color(194, 199, 162)); // Set the text color to #c2c7a2
    }
}