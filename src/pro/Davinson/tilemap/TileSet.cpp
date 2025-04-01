#include "TileSet.h"
#include <iostream>

bool TileSet::load(const std::string& tilesetPath) {
    if (!texture.loadFromFile(tilesetPath)) {
        std::cerr << "Error cargando el tileset: " << tilesetPath << "\n";
        return false;
    }
    return true;
}

void TileSet::addTile(int id, sf::IntRect rect, sf::Vector2u size) {
    tiles.emplace(id, Tile(id, rect, size));
}

const Tile* TileSet::getTile(int id) const {
    auto it = tiles.find(id);
    if (it != tiles.end()) {
        return &it->second;
    }
    return nullptr;
}
