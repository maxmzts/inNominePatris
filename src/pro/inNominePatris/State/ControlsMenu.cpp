#include "ControlsMenu.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "../Game.h"
#include <iostream>

ControlsMenu* ControlsMenu::instance = nullptr;

ControlsMenu::ControlsMenu(GameEngine& engine, float width, float height) : engine(engine) {
    // Load the controls.png image
    if (!loadingTexture.loadFromFile("./resources/controls.png")) {
        std::cerr << "Error loading controls.png" << std::endl;
    }
    loadingSprite.setTexture(loadingTexture);
    loadingSprite.setScale(width / loadingTexture.getSize().x, height / loadingTexture.getSize().y);

    // Load font
    if (!font.loadFromFile("./assets/fonts/PIXEL.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Back button setup
    backButtonText.setFont(font);
    backButtonText.setString("Volver");
    backButtonText.setCharacterSize(30);

    // Position the button at the bottom center
    float buttonX = (width - backButtonText.getLocalBounds().width) / 2;
    float buttonY = height - 100;
    backButtonText.setPosition(buttonX, buttonY);

    // Store the default colors
    defaultButtonTextColor = backButtonText.getFillColor();

    // Initialize the underline
    underline.setSize(sf::Vector2f(backButtonText.getLocalBounds().width, 2)); // Set the line's thickness
    underline.setFillColor(sf::Color(194, 199, 162)); // Set the line's color to #c2c7a2
    underline.setPosition(backButtonText.getPosition().x, backButtonText.getPosition().y + backButtonText.getLocalBounds().height + 8); // Position under the text with a small offset
    underlineVisible = false; // Initially hide the underline
}

ControlsMenu* ControlsMenu::getInstance(GameEngine& engine, float width, float height) {
    if (!instance) {
        instance = new ControlsMenu(engine, width, height);
    }
    return instance;
}

void ControlsMenu::update(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Mouse hover effect
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        if (backButtonText.getGlobalBounds().contains(worldPos)) {
            backButtonText.setFillColor(sf::Color(194, 199, 162)); // Set the text color to #c2c7a2
            underlineVisible = true; // Show the underline
        } else {
            backButtonText.setFillColor(defaultButtonTextColor);
            underlineVisible = false; // Hide the underline
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (backButtonText.getGlobalBounds().contains(worldPos)) {
                    game.changeState(MainMenu::getInstance(engine, 800, 600));
                }
            }
        }
    }
}

void ControlsMenu::render(Game& game, sf::RenderWindow& window) {
    window.clear();
    GameEngine& engine = game.getEngine();

    engine.drawSprite(loadingSprite);
    engine.drawText(backButtonText);
    if (underlineVisible) {
        window.draw(underline); // Draw the underline if it's visible
    }

    engine.display();
}