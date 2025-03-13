#include "TileMap.h"
#include <iostream>

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height) {
    if (!tilesetTexture.loadFromFile(tileset)) {
        std::cerr << "Error cargando el tileset\n";
        return false;
    }

    // Definir las coordenadas manualmente según la distribución en el tileset
    tileMap = {
        { 0, sf::IntRect(0, 0, 32, 32) },   
        { 1, sf::IntRect(208, 736, 32, 48) },  
        { 2, sf::IntRect(64, 0, 32, 32) },  
        { 3, sf::IntRect(96, 0, 32, 32) },  
        { 4, sf::IntRect(0, 32, 32, 32) },  
        { 5, sf::IntRect(32, 32, 32, 32) },  
        { 6, sf::IntRect(64, 32, 32, 32) },  
        { 7, sf::IntRect(96, 32, 32, 32) },
        { 8, sf::IntRect(0, 64, 32, 32) },
        { 9, sf::IntRect(32, 64, 32, 32) },
        { 10, sf::IntRect(64, 64, 32, 32) },
        { 11, sf::IntRect(96, 64, 32, 32) },
        { 12, sf::IntRect(0, 96, 32, 32) }
    };
    
    mapWidth = width;
    mapHeight = height;

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int tileNumber = tiles[x + y * width];

            // Verificar si el tile está en el mapa
            if (tileMap.find(tileNumber) == tileMap.end()) {
                std::cerr << "Tile " << tileNumber << " no encontrado en el mapa de tiles\n";
                continue;
            }

            sf::IntRect tileRect = tileMap[tileNumber];

            sf::Vertex* quad = &vertices[(x + y * width) * 4];

            quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
            quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
            quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tileRect.left, tileRect.top);
            quad[1].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top);
            quad[2].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top + tileRect.height);
            quad[3].texCoords = sf::Vector2f(tileRect.left, tileRect.top + tileRect.height);

            std::cout << "Tile " << tileNumber << " -> (" 
                      << tileRect.left << ", " << tileRect.top << ")\n";
        }
    }
    return true;
}

// 🔹 Implementación de la función draw (faltaba)
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &tilesetTexture;
    target.draw(vertices, states);
}
