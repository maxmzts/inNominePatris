#ifndef TILESET_H
#define TILESET_H

#include <SFML/Graphics.hpp>
#include <map>
#include "Tile.h"

class TileSet {
public:
    bool load(const std::string& tilesetPath);
    void addTile(int id, sf::IntRect rect, sf::Vector2u size);
    const Tile* getTile(int id) const;
    const sf::Texture& getTexture() const { return texture; }

private:
    sf::Texture texture;
    std::map<int, Tile> tiles;
};

#endif
