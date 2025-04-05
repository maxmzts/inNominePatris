#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameEngine {
private:
    sf::RenderWindow window;
    std::vector<sf::Sprite> spritesToDraw;
    std::map<std::string, sf::Texture> textures; // Mapa para almacenar texturas cargadas


    void initWindow(const std::string& title, int width, int height);

public:
    GameEngine(const std::string& title, int width, int height);
    ~GameEngine();

    void clear();
    void display();
    void drawSprite(const sf::Sprite& sprite);
    sf::Sprite createSprite(const std::string& texturePath, const sf::Vector2f& position);
    sf::Texture& loadTexture(const std::string& path);
    void drawVertices(const sf::VertexArray& vertices, const sf::Texture& texture, const sf::Transform& transform);
    bool isRunning() const; // Nuevo método
    void pollEvents();      // Nuevo método
};