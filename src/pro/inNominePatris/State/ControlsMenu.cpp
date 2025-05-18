#include "ControlsMenu.h"
#include "MainMenu.h"
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
    if (!font.loadFromFile("./assets/fonts/IMPACT.TTF")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Back button setup
    backButtonText.setFont(font);
    backButtonText.setString("Menu principal");
    backButtonText.setCharacterSize(30);

    // Background for the back button
    backButtonBackground.setSize(sf::Vector2f(300, 50));
    backButtonBackground.setFillColor(sf::Color(50, 50, 50, 200));
    backButtonBackground.setOutlineThickness(2);
    backButtonBackground.setOutlineColor(sf::Color::White);

    // Position the button at the bottom center
    float buttonX = (width - 300) / 2;
    float buttonY = height - 100;
    backButtonBackground.setPosition(buttonX, buttonY);

    // Center the text within the button
    sf::FloatRect textBounds = backButtonText.getLocalBounds();
    backButtonText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    backButtonText.setPosition(buttonX + 150, buttonY + 25 - 5);

    // Store the default colors
    defaultButtonTextColor = backButtonText.getFillColor();
    defaultButtonOutlineColor = backButtonBackground.getOutlineColor();
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

        if (backButtonBackground.getGlobalBounds().contains(worldPos)) {
            backButtonText.setFillColor(sf::Color::Red);
            backButtonBackground.setOutlineColor(sf::Color::Red);
        } else {
            backButtonText.setFillColor(defaultButtonTextColor);
            backButtonBackground.setOutlineColor(defaultButtonOutlineColor);
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (backButtonBackground.getGlobalBounds().contains(worldPos)) {
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
    engine.drawRectangle(backButtonBackground);
    engine.drawText(backButtonText);

    engine.display();
}