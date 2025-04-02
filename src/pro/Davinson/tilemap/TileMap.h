#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <tinyxml2.h>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool loadFromFile(const std::string& filename);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    struct TileSet {
        sf::Texture texture;
        int firstGid;
        int tileWidth, tileHeight;
        int columns;
    };

    struct Layer {
        sf::VertexArray vertices;
        sf::Texture* tilesetTexture;
    };

    std::map<int, TileSet> m_tilesets;  // Mapeo de firstGid a tilesets
    std::vector<Layer> m_layers;        // Lista de capas
    int m_mapWidth, m_mapHeight;
};

#endif
