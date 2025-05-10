// GameEngine.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map> // Faltaba este include
#include "SpriteFacade.h"

class GameEngine {
private:
    sf::RenderWindow window;
    sf:: RenderWindow* existingWindow; // Referencia a la ventana
    sf::View view; // Cámara
    std::vector<sf::Sprite> spritesToDraw;
    std::map<std::string, sf::Texture> textures;
    bool ownsWindow;

    sf::Clock clock; // Reloj para el tiempo de juego

    void initWindow(const std::string& title, int width, int height);

public:
    GameEngine(const std::string& title, int width, int height);

    GameEngine(sf::RenderWindow& windowRef);
    ~GameEngine();
    
    sf::RenderWindow& getWindow();
    void clear();
    void display();
    void drawSprite(const sf::Sprite& sprite);
    void drawSprite(const SpriteFacade& sprite);
    sf::Sprite createSprite(const std::string& texturePath, const sf::Vector2f& position);
    sf::Texture& loadTexture(const std::string& path);
    void drawVertices(const sf::VertexArray& vertices, const sf::Texture& texture, const sf::Transform& transform);
    bool isRunning() const;
    void pollEvents();
    sf::Vector2f getMousePosition() const;

    // --- NUEVO ---
    void setViewCenter(const sf::Vector2f& center);
    void resetView();
    float getDeltaTime();

    void drawRectangle(const sf::RectangleShape& rectangle);
    void drawText(const sf::Text& text);
    sf::RenderWindow& getRenderWindow(); // Para obtener el RenderWindow directamente
};
