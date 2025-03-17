#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

class TileMap : public sf::Drawable {
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height);

    // Mapa que almacena la posición de cada tile en la textura
    std::map<int, sf::IntRect> tileMap;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray vertices;
    sf::Texture tilesetTexture;
    unsigned int mapWidth, mapHeight;
};

#endif
