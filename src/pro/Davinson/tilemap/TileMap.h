#pragma once
#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include <vector>
#include <unordered_map>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool loadFromFile(const std::string& filename);
    bool isColliding(const sf::FloatRect& playerBounds) const; // NUEVA FUNCIÓN

private:
    struct TileSet {
        int firstGid;
        sf::Texture texture;
        int tileWidth, tileHeight, columns;
    };

    struct Layer {
        sf::VertexArray vertices;
        const sf::Texture* tilesetTexture;
    };

    std::vector<Layer> m_layers;
    std::unordered_map<int, TileSet> m_tilesets;
    int m_mapWidth, m_mapHeight;

    std::vector<sf::FloatRect> collisionBlocks; // ALMACENA LOS BLOQUES DE COLISIÓN

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
