#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameEngine {
private:
    sf::RenderWindow window;
    std::vector<sf::Sprite> spritesToDraw;

    void initWindow(const std::string& title, int width, int height);

public:
    GameEngine(const std::string& title, int width, int height);
    ~GameEngine();

    void clear();
    void display();
    void drawSprite(const sf::Sprite& sprite);
    sf::Sprite createSprite(const std::string& texturePath, const sf::Vector2f& position);
    bool isRunning() const; // Nuevo método
    void pollEvents();      // Nuevo método
};