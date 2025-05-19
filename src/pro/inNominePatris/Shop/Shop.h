#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "KarmaSystem.h"

class Shop {
public:
    Shop(sf::RenderWindow& window, KarmaSystem& karmaSystem);
    
    void open();
    void close();
    bool isOpen() const;
    void update(int playerKarma);
    void render(sf::RenderWindow& window);
    void handleInput(const sf::Event& event);
    bool shouldConnectNodes(size_t index1, size_t index2);
    std::string normalizarTexto(const std::string& texto);



private:
    void initializeTree();
    void createConnectionLines();
    void updateNodeVisuals();
    
    sf::RenderWindow& window;
    sf::Vector2u lastWindowSize;  // Tracks the last window size to detect changes
    KarmaSystem& karmaSystem;
    bool openState;
    
    sf::Font font;
    sf::Texture lockTexture;
    
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text descriptionText;
    sf::Text textoNormalizado;
    sf::Text karmaText;
    sf::Text pecadoTitleText;
    sf::Text absolucionTitleText;
    
    std::vector<sf::RectangleShape> nodes;
    std::vector<sf::Text> nodeTexts;
    std::vector<sf::Sprite> lockIcons;
    std::vector<sf::VertexArray> connectionLines;
    
    int selectedNodeIndex;
    sf::View previousView;
};