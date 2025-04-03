#include "TileMap.h"
#include <iostream>
#include <sstream>

bool TileMap::loadFromFile(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error cargando el mapa XML\n";
        return false;
    }

    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) return false;

    mapElement->QueryIntAttribute("width", &m_mapWidth);
    mapElement->QueryIntAttribute("height", &m_mapHeight);

    TileSet* mainTileset = nullptr;
    TileSet* decoTileset = nullptr;

    for (tinyxml2::XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
         tilesetElement; tilesetElement = tilesetElement->NextSiblingElement("tileset")) {
        
        int firstGid;
        tilesetElement->QueryIntAttribute("firstgid", &firstGid);
        
        tinyxml2::XMLElement* imageElement = tilesetElement->FirstChildElement("image");
        if (!imageElement) continue;
        
        const char* source = imageElement->Attribute("source");
        if (!source) continue;

        TileSet tileset;
        tileset.firstGid = firstGid;
        tileset.texture.loadFromFile(std::string("resources/") + source);
        tilesetElement->QueryIntAttribute("tilewidth", &tileset.tileWidth);
        tilesetElement->QueryIntAttribute("tileheight", &tileset.tileHeight);
        tileset.columns = tileset.texture.getSize().x / tileset.tileWidth;

        m_tilesets[firstGid] = tileset;

        if (!mainTileset) {
            mainTileset = &m_tilesets[firstGid];
        } else {
            decoTileset = &m_tilesets[firstGid];
        }
    }

    for (tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
         layerElement; layerElement = layerElement->NextSiblingElement("layer")) {
        
        const char* layerName = layerElement->Attribute("name");
        bool isDecoLayer = (layerName && std::string(layerName) == "deco");
        bool isBoundsLayer = (layerName && std::string(layerName) == "bounds");

        tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement) continue;

        std::vector<int> tiles;
        const char* csvData = dataElement->GetText();
        std::stringstream ss(csvData);
        std::string tileID;
        
        while (std::getline(ss, tileID, ',')) {
            tiles.push_back(std::stoi(tileID));
        }

        if (isBoundsLayer) {
            for (int y = 0; y < m_mapHeight; ++y) {
                for (int x = 0; x < m_mapWidth; ++x) {
                    int tileNumber = tiles[y * m_mapWidth + x];
                    if (tileNumber != 0) {
                        collisionBlocks.push_back(sf::FloatRect(x * mainTileset->tileWidth, y * mainTileset->tileHeight,
                                                                mainTileset->tileWidth, mainTileset->tileHeight));
                    }
                }
            }
            continue; 
        }

        Layer layer;
        layer.vertices.setPrimitiveType(sf::Quads);
        layer.vertices.resize(m_mapWidth * m_mapHeight * 4);

        for (int y = 0; y < m_mapHeight; ++y) {
            for (int x = 0; x < m_mapWidth; ++x) {
                int tileNumber = tiles[y * m_mapWidth + x];
                if (tileNumber == 0) continue;

                TileSet* tileset = isDecoLayer ? decoTileset : mainTileset;
                if (!tileset) continue;

                int relativeTileID = tileNumber - tileset->firstGid;
                int tu = relativeTileID % tileset->columns;
                int tv = relativeTileID / tileset->columns;

                sf::Vertex* quad = &layer.vertices[(x + y * m_mapWidth) * 4];

                quad[0].position = sf::Vector2f(x * tileset->tileWidth, y * tileset->tileHeight);
                quad[1].position = sf::Vector2f((x + 1) * tileset->tileWidth, y * tileset->tileHeight);
                quad[2].position = sf::Vector2f((x + 1) * tileset->tileWidth, (y + 1) * tileset->tileHeight);
                quad[3].position = sf::Vector2f(x * tileset->tileWidth, (y + 1) * tileset->tileHeight);

                quad[0].texCoords = sf::Vector2f(tu * tileset->tileWidth, tv * tileset->tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, tv * tileset->tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, (tv + 1) * tileset->tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * tileset->tileWidth, (tv + 1) * tileset->tileHeight);
            }
        }

        layer.tilesetTexture = isDecoLayer ? &decoTileset->texture : &mainTileset->texture;
        m_layers.push_back(layer);
    }

    return true;
}

bool TileMap::isColliding(const sf::FloatRect& playerBounds) const {
    for (const auto& block : collisionBlocks) {
        if (playerBounds.intersects(block)) {
            return true;
        }
    }
    return false;
}

// Implementación de la función `draw`, necesaria para evitar errores de vtable
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& layer : m_layers) {
        if (layer.tilesetTexture) {
            states.texture = layer.tilesetTexture;
            target.draw(layer.vertices, states);
        }
    }
}
